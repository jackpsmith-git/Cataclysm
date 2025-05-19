#include "ccpch.h"
#include "ProfilerPanel.h"

#include <imgui/imgui.h>

namespace Cataclysm
{
	void ProfileSession::Load()
	{
		Events.clear();
		MinTimestamp = UINT64_MAX;
		MaxTimestamp = 0;
		Threads.clear();
		_IsDraggingScroll = false;
		_ScrollDragOffsetX = 0.0f;
		OffsetX = 0.0f;
		Zoom = 0.1f;
		ParseFile();
		ExtractThreads();
		ComputeNestingLevels();

		if (Zoom < 0.005f || !std::isfinite(Zoom))
			Zoom = 0.1f;

		if (MaxTimestamp < MinTimestamp)
			std::swap(MinTimestamp, MaxTimestamp);
	}

	void ProfileSession::ComputeNestingLevels()
	{
		EventsByThread.clear();
		MaxNestingLevel = 1.0f;

		// Group events by thread
		for (const auto& ev : Events)
			EventsByThread[ev.ThreadID].push_back(EventWithLevel(ev, -1));

		// Sort and compute nesting per thread
		for (auto& [thread, evs] : EventsByThread)
		{
			std::sort(evs.begin(), evs.end(), [](const EventWithLevel& a, const EventWithLevel& b) {
				return a.Event.Timestamp < b.Event.Timestamp;
				});

			std::vector<uint64_t> stackEndTimes; // active event end times per nesting level

			for (auto& e : evs)
			{
				while (!stackEndTimes.empty() && stackEndTimes.back() <= e.Event.Timestamp)
					stackEndTimes.pop_back();

				e.NestingLevel = (int)stackEndTimes.size();
				MaxNestingLevel = std::max(MaxNestingLevel, float(e.NestingLevel + 1));
				stackEndTimes.push_back(e.Event.Timestamp + e.Event.Duration);
			}
		}
	}

	void ProfileSession::ParseFile()
	{
		if (!std::filesystem::exists(FilePath))
		{
			CC_CORE_WARN("Profiler session file not found: " + FilePath);
			return;
		}

		std::ifstream file(FilePath);
		if (!file.is_open()) {
			CC_CORE_WARN("Failed to open file: " + FilePath);
			return;
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string contents = buffer.str();

		size_t pos = 0;
		int count = 0;
		while ((pos = contents.find("{", pos)) != std::string::npos)
		{
			TraceEvent ev{};

			auto GetValue = [&](const std::string& key) -> std::string {
				size_t k = contents.find("\"" + key + "\":", pos);
				if (k == std::string::npos) return "";
				size_t start = contents.find_first_of("0123456789\"", k + key.length() + 3);
				if (start == std::string::npos) return "";
				if (contents[start] == '"') // string value
				{
					size_t end = contents.find("\"", start + 1);
					return contents.substr(start + 1, end - start - 1);
				}
				else // numeric value
				{
					size_t end = contents.find_first_not_of("0123456789", start);
					return contents.substr(start, end - start);
				}
				};

			ev.Name = GetValue("name");
			std::string ts = GetValue("ts");
			std::string dur = GetValue("dur");
			std::string tid = GetValue("tid");

			if (!ts.empty()) ev.Timestamp = std::stoull(ts);
			if (!dur.empty()) ev.Duration = std::stoull(dur);
			if (!tid.empty()) ev.ThreadID = std::stoul(tid);

			Events.push_back(ev);
			MinTimestamp = std::min(MinTimestamp, ev.Timestamp);
			MaxTimestamp = std::max(MaxTimestamp, ev.Timestamp + ev.Duration);
			count++;

			pos = contents.find("}", pos);
			if (pos != std::string::npos) pos++;
		}
	}

	void ProfileSession::ExtractThreads()
	{
		for (const auto& ev : Events)
		{
			if (std::find(Threads.begin(), Threads.end(), ev.ThreadID) == Threads.end())
				Threads.push_back(ev.ThreadID);
		}
	}

	void ProfilerPanel::Init()
	{
		m_Sessions = {
			{ "Startup", "CataclysmProfile-Startup.json" },
			{ "Runtime", "CataclysmProfile-Runtime.json" },
			{ "Shutdown", "CataclysmProfile-Shutdown.json" }
		};
		for (auto& s : m_Sessions) s.Load();
	}

	static ImU32 GetColorForEventName(const std::string& name)
	{
		uint32_t hash = 5381;
		for (char c : name)
			hash = ((hash << 5) + hash) + c; // djb2 hash

		uint8_t r = (hash & 0xFF0000) >> 16;
		uint8_t g = (hash & 0x00FF00) >> 8;
		uint8_t b = (hash & 0x0000FF);

		// Lighten the color so it’s not too dark
		r = (uint8_t)((r + 128) / 2);
		g = (uint8_t)((g + 128) / 2);
		b = (uint8_t)((b + 128) / 2);

		return IM_COL32(r, g, b, 180);
	}

	void ProfilerPanel::OnImGuiRender()
	{
		ImGui::Begin("Profiler");

		if (ImGui::BeginTabBar("ProfilerTabs"))
		{
			for (auto& session : m_Sessions)
			{
				if (ImGui::BeginTabItem(session.Name.c_str()))
				{
					float availableWidth = ImGui::GetContentRegionAvail().x;
					float timelineWidth = session.MaxTimestamp - session.MinTimestamp;
					float minZoom = 0.002f;
					float maxZoom = 0.05f;

					// Controls
					if (ImGui::Button("Reload"))
						session.Load();

					ImGui::SameLine();

					// --- Handle zooming while preserving view center ---
					float oldZoom = session.Zoom;
					float visibleStart = -session.OffsetX; // left edge in timeline space

					if (ImGui::SliderFloat("Zoom", &session.Zoom, minZoom, maxZoom, "%.4f"))
					{
						float zoomRatio = session.Zoom / oldZoom;

						// Recalculate OffsetX to preserve the visible start (left edge)
						session.OffsetX = -visibleStart * zoomRatio;
					}

					auto& eventsByThread = session.EventsByThread;

					// Determine max nesting per thread for sizing
					float maxNestingLevel = 1.0f;
					for (const auto& [tid, evs] : eventsByThread)
					{
						for (const auto& ev : evs)
						{
							maxNestingLevel = session.MaxNestingLevel;;
						}
					}

					const float laneHeight = 28.0f;
					const float laneSpacing = 6.0f;
					float baseThreadHeight = (laneHeight + laneSpacing) * maxNestingLevel + 10.0f;

					// Clamp canvas height (or make scrollable)
					float canvasHeight = baseThreadHeight * session.Threads.size();
					const float maxCanvasHeight = 1000.0f;
					if (canvasHeight > maxCanvasHeight)
						canvasHeight = maxCanvasHeight;

					ImVec2 canvasSize(ImGui::GetContentRegionAvail().x, canvasHeight);

					ImDrawList* dl = ImGui::GetWindowDrawList();
					ImVec2 cp = ImGui::GetCursorScreenPos();

					// Draw background for the entire canvas
					dl->AddRectFilled(cp, ImVec2(cp.x + canvasSize.x, cp.y + canvasSize.y), IM_COL32(40, 40, 40, 255));

					// Draw per-thread lanes and events
					float currentY = cp.y;

					for (uint32_t threadID : session.Threads)
					{
						// Draw thread background lane
						dl->AddRectFilled(ImVec2(cp.x, currentY), ImVec2(cp.x + canvasSize.x, currentY + baseThreadHeight), IM_COL32(50, 50, 50, 60));

						// Draw events on this thread
						if (eventsByThread.find(threadID) != eventsByThread.end())
						{
							const auto& evs = eventsByThread[threadID];
							for (const auto& e : evs)
							{
								float x = cp.x + (e.Event.Timestamp - session.MinTimestamp) * session.Zoom + session.OffsetX;
								float w = e.Event.Duration * session.Zoom;

								if (w < 1.0f)
									w = 1.0f;

								float y = currentY + e.NestingLevel * (laneHeight + laneSpacing);

								ImVec2 p0(x, y);
								ImVec2 p1(x + w, y + laneHeight);

								if (p1.x < cp.x || p0.x > cp.x + canvasSize.x)
									continue;

								ImU32 color = GetColorForEventName(e.Event.Name);
								dl->AddRectFilled(p0, p1, color, 3.0f);
								dl->AddRect(p0, p1, IM_COL32(0, 0, 0, 120), 3.0f);

								if (w > 20.0f)
								{
									std::string text = e.Event.Name;
									ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

									if (textSize.x > w - 6)
									{
										int maxChars = (int)((w - 12) / ImGui::CalcTextSize("A").x);
										if (maxChars > 3 && maxChars < (int)text.size())
											text = text.substr(0, maxChars - 3) + "...";

										// Remove this line to not show trucated labels at all
										// dl->AddText(ImVec2(x + 3, y + 2), IM_COL32(255, 255, 255, 230), text.c_str());
									}

									if (textSize.x <= w - 6)
									{
										dl->AddText(ImVec2(x + 3, y + 2), IM_COL32(255, 255, 255, 230), text.c_str());
									}
								}

								if (ImGui::IsMouseHoveringRect(p0, p1))
								{
									ImGui::BeginTooltip();
									ImGui::TextUnformatted(e.Event.Name.c_str());
									ImGui::Text("Duration: %.3f ms", e.Event.Duration / 1000.0);
									ImGui::Text("Thread ID: %u", e.Event.ThreadID);
									ImGui::EndTooltip();
								}
							}
						}

						currentY += baseThreadHeight;
					}

					ImGui::SetCursorScreenPos(ImVec2(cp.x, cp.y + canvasSize.y));

					float contentWidth = (session.MaxTimestamp - session.MinTimestamp) * session.Zoom;
					float visibleWidth = canvasSize.x;

					float scrollMin = std::min(0.0f, visibleWidth - contentWidth); // allow 0 if content fits
					float scrollMax = 0.0f;

					// Clamp scroll position
					session.OffsetX = std::clamp(session.OffsetX, scrollMin, 0.0f);

					// UI: Manual horizontal scrollbar
					float scrollableWidth = std::max(1.0f, contentWidth - visibleWidth);
					float scrollRatio = -session.OffsetX / scrollableWidth;
					float scrollBarWidth = visibleWidth * (visibleWidth / contentWidth);
					scrollBarWidth = std::clamp(scrollBarWidth, 8.0f, visibleWidth); // 8.0f min size

					float scrollBarX = ImGui::GetCursorScreenPos().x + scrollRatio * (visibleWidth - scrollBarWidth);

					// Draw scrollbar track
					ImVec2 sbPos = ImGui::GetCursorScreenPos();
					ImVec2 sbEnd(sbPos.x + visibleWidth, sbPos.y + 12.0f);
					ImGui::Dummy(ImVec2(visibleWidth, 12.0f)); // Reserve space
					// ImDrawList* dl = ImGui::GetWindowDrawList();
					dl->AddRectFilled(sbPos, sbEnd, IM_COL32(60, 60, 60, 180), 4.0f);

					// Draw scroll thumb
					ImVec2 thumbPos(scrollBarX, sbPos.y);
					ImVec2 thumbEnd(scrollBarX + scrollBarWidth, sbPos.y + 12.0f);
					dl->AddRectFilled(thumbPos, thumbEnd, IM_COL32(120, 120, 200, 255), 4.0f);

					// Handle dragging
					ImVec2 mouse = ImGui::GetIO().MousePos;
					bool hovered = ImGui::IsMouseHoveringRect(thumbPos, thumbEnd);
					if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
						session._IsDraggingScroll = true;

					if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
						session._IsDraggingScroll = false;

					if (session._IsDraggingScroll)
					{
						float delta = ImGui::GetIO().MouseDelta.x;
						delta *= ((200 * session.Zoom)); // FIX THIS
						session.OffsetX -= delta;
						session.OffsetX = std::clamp(session.OffsetX, -contentWidth + visibleWidth, 0.0f);
					}

					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
	}
}
