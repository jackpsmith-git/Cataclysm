#include "ccpch.h"
#include "Panels/CPOutputPanel.h"

#include "Cataclysm/Core/CCLog.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "ImGui/CPImGui.h"
#include "CPResources.h"
#include "CPStyles.h"

namespace Cataclysm
{
	void CPOutputPanel::OnImGuiRender()
	{
		CPImGui::BeginPanel("Output");
		CPImGui::PanelBackground();
		CPImGui::BeginPanelMenu();
		{
			ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - ((6 * CPStyles::GetPanelMenuHeight()) + (3 * 1)));

			float size = CPStyles::GetPanelMenuHeight();

			if (CPImGui::ImageButton(CPResources::ICON_CLEAR, size))
			{
				std::ofstream file(CCLog::GetLogFilePath().string().c_str(), std::ios::out);
				if (file.is_open())
				{
					file.close();
				}
			}

			ImGui::SameLine();

			if (m_ShowTimestamps)
			{
				if (CPImGui::ImageButton(CPResources::ICON_TIMESTAMP, size))
				{
					m_ShowTimestamps = false;
				}
			}
			else
			{
				if (CPImGui::ImageButton(CPResources::ICON_TIMESTAMP_FADED, size))
				{
					m_ShowTimestamps = true;
				}
			}

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(3, 0));

			ImGui::SameLine();
			if (errorsEnabled)
			{
				if (CPImGui::ImageButton(CPResources::ICON_ERROR, size))
				{
					errorsEnabled = false;
				}
			}
			else
			{
				if (CPImGui::ImageButton(CPResources::ICON_ERROR_FADED, size))
				{
					errorsEnabled = true;
				}
			}

			ImGui::PopStyleVar();
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

			ImGui::SameLine();
			if (warningsEnabled)
			{
				if (CPImGui::ImageButton(CPResources::ICON_WARNING, size))
				{
					warningsEnabled = false;
				}
			}
			else
			{
				if (CPImGui::ImageButton(CPResources::ICON_WARNING_FADED, size))
				{
					warningsEnabled = true;
				}
			}

			ImGui::SameLine();
			if (traceEnabled)
			{
				if (CPImGui::ImageButton(CPResources::ICON_TRACE, size))
				{
					traceEnabled = false;
				}
			}
			else
			{
				if (CPImGui::ImageButton(CPResources::ICON_TRACE_FADED, size))
				{
					traceEnabled = true;
				}
			}

			ImGui::SameLine();
			if (infoEnabled)
			{
				if (CPImGui::ImageButton(CPResources::ICON_INFO, size))
				{
					infoEnabled = false;
				}
			}
			else
			{
				if (CPImGui::ImageButton(CPResources::ICON_INFO_FADED, size))
				{
					infoEnabled = true;
				}
			}
		}
		CPImGui::EndPanelMenu();

		ImGui::PopStyleVar();

		CPImGui::BeginPanelContent();

		std::ifstream file(CCLog::GetLogFilePath().string().c_str());

		if (!file.is_open())
		{
			CC_CORE_ERROR("Error opening log file!");
			return;
		}

		std::string line;
		while (std::getline(file, line))
		{
			if (line.find("warning") != std::string::npos)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, CPStyles::TEXT_COLOR_OUTPUT_WARNING);
				
				if (warningsEnabled)
				{
					std::string substr = "[warning] ";
					size_t pos = line.find(substr);
					line.erase(pos, substr.length());

					if (!m_ShowTimestamps)
					{
						if (!line.empty() && line[0] == '[')
						{
							size_t closeBracket = line.find(']');
							if (closeBracket != std::string::npos)
							{
								size_t removeCount = closeBracket + 1;
								if (line.size() > removeCount && line[removeCount] == ' ')
									removeCount++;

								line.erase(0, removeCount);
							}
						}
					}

					ImGui::TextWrapped(line.c_str());
				}
			}
			else if (line.find("error") != std::string::npos)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, CPStyles::TEXT_COLOR_OUTPUT_ERROR);

				if (errorsEnabled)
				{
					std::string substr = "[error] ";
					size_t pos = line.find(substr);
					line.erase(pos, substr.length());

					if (!m_ShowTimestamps)
					{
						if (!line.empty() && line[0] == '[')
						{
							size_t closeBracket = line.find(']');
							if (closeBracket != std::string::npos)
							{
								size_t removeCount = closeBracket + 1;
								if (line.size() > removeCount && line[removeCount] == ' ')
									removeCount++;

								line.erase(0, removeCount);
							}
						}
					}

					ImGui::TextWrapped(line.c_str());
				}
			}
			else if (line.find("trace") != std::string::npos)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, CPStyles::TEXT_COLOR_OUTPUT_TRACE);

				if (traceEnabled)
				{
					std::string substr = "[trace] ";
					size_t pos = line.find(substr);
					line.erase(pos, substr.length());

					if (!m_ShowTimestamps)
					{
						if (!line.empty() && line[0] == '[')
						{
							size_t closeBracket = line.find(']');
							if (closeBracket != std::string::npos)
							{
								size_t removeCount = closeBracket + 1;
								if (line.size() > removeCount && line[removeCount] == ' ')
									removeCount++;

								line.erase(0, removeCount);
							}
						}
					}

					ImGui::TextWrapped(line.c_str());
				}
			}
			else if (line.find("info") != std::string::npos)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, CPStyles::TEXT_COLOR_OUTPUT_INFO);

				if (infoEnabled)
				{
					std::string substr = "[info] ";
					size_t pos = line.find(substr);
					line.erase(pos, substr.length());

					if (!m_ShowTimestamps)
					{
						if (!line.empty() && line[0] == '[')
						{
							size_t closeBracket = line.find(']');
							if (closeBracket != std::string::npos)
							{
								size_t removeCount = closeBracket + 1;
								if (line.size() > removeCount && line[removeCount] == ' ')
									removeCount++;

								line.erase(0, removeCount);
							}
						}
					}

					ImGui::TextWrapped(line.c_str());
				}
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, CPStyles::TEXT_COLOR_OUTPUT_DEFAULT);

				if (!m_ShowTimestamps)
				{
					if (!line.empty() && line[0] == '[')
					{
						size_t closeBracket = line.find(']');
						if (closeBracket != std::string::npos)
						{
							size_t removeCount = closeBracket + 1;
							if (line.size() > removeCount && line[removeCount] == ' ')
								removeCount++;

							line.erase(0, removeCount);
						}
					}
				}

				ImGui::TextWrapped(line.c_str());
			}

			ImGui::PopStyleColor();
		}

		// Extra space to avoid overlap with status bar
		ImGui::TextWrapped(" ");
		ImGui::TextWrapped(" ");
		ImGui::TextWrapped(" ");
		ImGui::TextWrapped(" ");

		file.close();

		CPImGui::EndPanelContent();
		CPImGui::EndPanel();
	}
}
