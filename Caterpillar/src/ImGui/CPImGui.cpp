#include "ccpch.h"
#include "ImGui/CPImGui.h"

#include "Cataclysm/Vesuvius/VSTexture.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"

#include "CPStyles.h"

namespace Cataclysm::CPImGui
{
	bool ColorSelect(const char* label, float* color, float columnWidth)
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 1 });

		const float lineHeight = (ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f) * CPStyles::GetUIScale();
		const float spacing = 4.0f;

		ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - lineHeight - spacing);

		std::string colorSelectID = "##selectID" + std::string(label);
		bool returnValue = ImGui::ColorEdit4(colorSelectID.c_str(), color);

		ImGui::PopStyleVar(2);
		ImGui::Columns(1);
		return returnValue;
	}

	bool InputTextMultiline(const char* label, std::string* string, float columnWidth)
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 1 });

		const float lineHeight = (ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f) * CPStyles::GetUIScale();
		const float spacing = 4.0f;

		ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - lineHeight - spacing);

		std::string textID = "##textID" + std::string(label);
		bool returnValue = ImGui::InputTextMultiline(textID.c_str(), string, ImVec2(ImGui::GetContentRegionAvail().x - lineHeight - spacing, ImGui::GetTextLineHeight() * 3.0f));

		ImGui::PopStyleVar(2);
		ImGui::Columns(1);
		return returnValue;
	}

	bool InputDouble(const char* label, double* value, float columnWidth)
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 1 });

		ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

		std::string doubleID = "##doublelabel" + std::string(label);
		bool returnValue = ImGui::InputDouble(doubleID.c_str(), value);

		ImGui::PopStyleVar(2);
		ImGui::Columns(1);
		return returnValue;
	}

	bool BeginCombo(const char* label, const char* value, float columnWidth)
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 1 });

		const float lineHeight = (ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f) * CPStyles::GetUIScale();
		const float spacing = 4.0f;

		ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - lineHeight - spacing);

		std::string comboID = "##checkboxlabel" + std::string(label);
		bool returnValue = ImGui::BeginCombo(comboID.c_str(), value);
		ImGui::PopStyleVar(2);
		return returnValue;
	}

	bool DragVec2(const std::string& label, const std::string& uniqueID, glm::vec2& values, float resetValue, float columnWidth)
	{
		bool edited = false;

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImGui::PushID(uniqueID.c_str());
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ CPStyles::GetVectorControlFramePadding(), 1});

		const float spacing = CPStyles::GetVectorControlSpacing();
		const float rounding = CPStyles::GetElementRounding();
		const float lineHeight = (ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f) * CPStyles::GetUIScale();
		const float buttonWidth = lineHeight + 3.0f;
		float totalWidth = ImGui::GetContentRegionAvail().x - 5.0f;
		float totalSpacing = spacing * 5.0f;
		float remainingWidth = totalWidth - ((2 * buttonWidth) + lineHeight) - totalSpacing;
		float xDragWidth = remainingWidth / 2.0f;
		float yDragWidth = (remainingWidth - xDragWidth);

		ImVec2 startPos = ImGui::GetCursorScreenPos();
		ImVec2 endPos = { startPos.x + totalWidth + 5.0f - lineHeight - spacing, startPos.y + lineHeight };
		drawList->AddRectFilled(startPos, endPos, ImColor(50, 50, 50, 255), rounding);
		ImGui::SetCursorScreenPos(ImVec2(startPos.x, startPos.y));
		ImGui::BeginGroup();

		auto DrawRoundedButton = [&](const char* label, float* value, const ImVec4& color, ImDrawFlags roundFlags)
			{
				ImVec2 btnPos = ImGui::GetCursorScreenPos();
				ImVec2 btnSize = ImVec2(buttonWidth, lineHeight);

				ImGui::InvisibleButton(label, btnSize);
				bool hovered = ImGui::IsItemHovered();
				bool held = ImGui::IsItemActive();

				ImU32 baseColor = ImGui::GetColorU32(color);
				ImU32 hoveredColor = ImGui::GetColorU32(ImVec4(color.x + 0.1f, color.y + 0.1f, color.z + 0.1f, 1.0f));
				ImU32 activeColor = ImGui::GetColorU32(ImVec4(color.x - 0.1f, color.y - 0.1f, color.z - 0.1f, 1.0f));

				ImU32 drawColor = baseColor;
				if (held) drawColor = activeColor;
				else if (hovered) drawColor = hoveredColor;

				drawList->AddRectFilled(btnPos, ImVec2(btnPos.x + buttonWidth, btnPos.y + lineHeight), drawColor, rounding, roundFlags);

				ImVec2 textSize = ImGui::CalcTextSize(label);
				ImVec2 textPos = ImVec2(btnPos.x + (buttonWidth - textSize.x) * 0.5f, btnPos.y + (lineHeight - textSize.y) * 0.5f);

				drawList->AddText(boldFont, ImGui::GetFontSize(), textPos, ImGui::GetColorU32(ImGuiCol_Text), label);

				if (ImGui::IsItemClicked())
					*value = resetValue;

				ImGui::SameLine(0.0f, spacing);
			};

		auto DrawDrag = [&](const char* id, float* value, float width, int axis)
			{
				ImGui::SetNextItemWidth(width);

				glm::vec2 original = values;

				if (ImGui::DragFloat(id, value, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					edited = true;

					ImGuiID lockID = ImGui::GetID("LockState");
					bool* locked = ImGui::GetStateStorage()->GetBoolRef(lockID, false);

					if (*locked)
					{
						float originalComponent = 0.0f;
						float newComponent = 0.0f;

						switch (axis)
						{
						case 0: originalComponent = original.x; newComponent = values.x; break;
						case 1: originalComponent = original.y; newComponent = values.y; break;
						}

						float ratio = (originalComponent != 0.0f) ? newComponent / originalComponent : 1.0f;

						switch (axis)
						{
						case 0:
							if (original.y != 0.0f) values.y = original.y * ratio;
							break;
						case 1:
							if (original.x != 0.0f) values.x = original.x * ratio;
							break;
							break;
						}
					}
				}

				ImGui::SameLine(0.0f, spacing);
			};

		DrawRoundedButton("X", &values.x, CPStyles::BUTTON_COLOR_X, ImDrawFlags_RoundCornersLeft);
		DrawDrag("##X", &values.x, xDragWidth, 0);
		DrawRoundedButton("Y", &values.y, CPStyles::BUTTON_COLOR_Y, ImDrawFlags_RoundCornersNone);
		DrawDrag("##Y", &values.y, yDragWidth, 1);

		// Lock button
		ImGui::Spacing();
		ImGui::SameLine();

		ImGui::SetCursorPosX(0.0f);
		ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - lineHeight);
		ImGuiID lockID = ImGui::GetID("LockState");
		bool* locked = ImGui::GetStateStorage()->GetBoolRef(lockID, false);
		ImVec2 lockBtnSize = { lineHeight, lineHeight };
		auto& lockIcon = *locked ? CPResources::ICON_LOCK : CPResources::ICON_UNLOCK;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		if (CPImGui::ImageButton(lockIcon, lockBtnSize))
			*locked = !*locked;
		ImGui::PopStyleColor(3);

		ImGui::EndGroup();
		ImGui::PopStyleVar(2); // ItemSpacing
		ImGui::Columns(1);
		ImGui::PopID();
		ImGui::Dummy(ImVec2(0, 0.0f));

		return edited;
	}

	bool DragVec3(const std::string& label, const std::string& uniqueID, glm::vec3& values, float resetValue, float columnWidth)
	{
		bool edited = false;

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImGui::PushID(uniqueID.c_str());
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ CPStyles::GetVectorControlFramePadding(), 1});

		const float spacing = CPStyles::GetVectorControlSpacing();
		const float rounding = CPStyles::GetElementRounding();
		const float lineHeight = (ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f) * CPStyles::GetUIScale();
		const float buttonWidth = lineHeight + 3.0f;
		float totalWidth = ImGui::GetContentRegionAvail().x - 5.0f;
		float totalSpacing = spacing * 6.0f; // 3 buttons + 3 sliders + lock button = 7 items, 6 spaces
		float remainingWidth = totalWidth - ((3 * buttonWidth) + lineHeight) - totalSpacing;
		float xDragWidth = remainingWidth / 3.0f;
		float yDragWidth = (remainingWidth - xDragWidth) / 2.0f;
		float zDragWidth = remainingWidth - xDragWidth - yDragWidth;

		ImVec2 startPos = ImGui::GetCursorScreenPos();
		ImVec2 endPos = { startPos.x + totalWidth + 5.0f - lineHeight - spacing, startPos.y + lineHeight };
		drawList->AddRectFilled(startPos, endPos, ImColor(50, 50, 50, 255), rounding);
		ImGui::SetCursorScreenPos(startPos);
		ImGui::BeginGroup();

		auto DrawRoundedButton = [&](const char* label, float* value, const ImVec4& color, ImDrawFlags roundFlags)
			{
				ImVec2 btnPos = ImGui::GetCursorScreenPos();
				ImVec2 btnSize = ImVec2(buttonWidth, lineHeight);

				ImGui::InvisibleButton(label, btnSize);
				bool hovered = ImGui::IsItemHovered();
				bool held = ImGui::IsItemActive();

				ImU32 baseColor = ImGui::GetColorU32(color);
				ImU32 hoveredColor = ImGui::GetColorU32(ImVec4(color.x + 0.1f, color.y + 0.1f, color.z + 0.1f, 1.0f));
				ImU32 activeColor = ImGui::GetColorU32(ImVec4(color.x - 0.1f, color.y - 0.1f, color.z - 0.1f, 1.0f));

				ImU32 drawColor = baseColor;
				if (held) drawColor = activeColor;
				else if (hovered) drawColor = hoveredColor;

				drawList->AddRectFilled(btnPos, ImVec2(btnPos.x + buttonWidth, btnPos.y + lineHeight), drawColor, rounding, roundFlags);

				ImVec2 textSize = ImGui::CalcTextSize(label);
				ImVec2 textPos = ImVec2(btnPos.x + (buttonWidth - textSize.x) * 0.5f, btnPos.y + (lineHeight - textSize.y) * 0.5f);

				drawList->AddText(boldFont, ImGui::GetFontSize(), textPos, ImGui::GetColorU32(ImGuiCol_Text), label);

				if (ImGui::IsItemClicked())
					*value = resetValue;

				ImGui::SameLine(0.0f, spacing);
			};

		auto DrawDrag = [&](const char* id, float* value, float width, int axis)
			{
				ImGui::SetNextItemWidth(width);

				glm::vec3 original = values; // Capture before any changes

				if (ImGui::DragFloat(id, value, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					edited = true;

					ImGuiID lockID = ImGui::GetID("LockState");
					bool* locked = ImGui::GetStateStorage()->GetBoolRef(lockID, false);

					if (*locked)
					{
						float originalComponent = 0.0f;
						float newComponent = 0.0f;

						switch (axis)
						{
						case 0: originalComponent = original.x; newComponent = values.x; break;
						case 1: originalComponent = original.y; newComponent = values.y; break;
						case 2: originalComponent = original.z; newComponent = values.z; break;
						}

						float ratio = (originalComponent != 0.0f) ? newComponent / originalComponent : 1.0f;

						switch (axis)
						{
						case 0:
							if (original.y != 0.0f) values.y = original.y * ratio;
							if (original.z != 0.0f) values.z = original.z * ratio;
							break;
						case 1:
							if (original.x != 0.0f) values.x = original.x * ratio;
							if (original.z != 0.0f) values.z = original.z * ratio;
							break;
						case 2:
							if (original.x != 0.0f) values.x = original.x * ratio;
							if (original.y != 0.0f) values.y = original.y * ratio;
							break;
						}
					}
				}

				ImGui::SameLine(0.0f, spacing);
			};

		// Drag controls and buttons
		DrawRoundedButton("X", &values.x, CPStyles::BUTTON_COLOR_X, ImDrawFlags_RoundCornersLeft);
		DrawDrag("##X", &values.x, xDragWidth, 0);
		DrawRoundedButton("Y", &values.y, CPStyles::BUTTON_COLOR_Y, ImDrawFlags_RoundCornersNone);
		DrawDrag("##Y", &values.y, yDragWidth, 1);
		DrawRoundedButton("Z", &values.z, CPStyles::BUTTON_COLOR_Z, ImDrawFlags_RoundCornersNone);
		DrawDrag("##Z", &values.z, zDragWidth, 2);

		// Lock button
		ImGui::Spacing();
		ImGui::SameLine();

		ImGui::SetCursorPosX(0.0f);
		ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - lineHeight);
		ImGuiID lockID = ImGui::GetID("LockState");
		bool* locked = ImGui::GetStateStorage()->GetBoolRef(lockID, false);
		ImVec2 lockBtnSize = { lineHeight, lineHeight };
		auto& lockIcon = *locked ? CPResources::ICON_LOCK : CPResources::ICON_UNLOCK;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		if (CPImGui::ImageButton(lockIcon, lockBtnSize))
			*locked = !*locked;
		ImGui::PopStyleColor(3);

		// ImGui Cleanup
		ImGui::EndGroup();
		ImGui::PopStyleVar(2);
		ImGui::Columns(1);
		ImGui::PopID();
		ImGui::Dummy(ImVec2(0, 0.0f));

		return edited;
	}

	bool DragVec4(const std::string& label, const std::string& uniqueID, glm::vec4& values, float resetValue, float columnWidth)
	{
		bool edited = false;

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImGui::PushID(label.c_str());
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ CPStyles::GetVectorControlFramePadding(), 1});

		const float spacing = CPStyles::GetVectorControlSpacing();
		const float rounding = CPStyles::GetElementRounding();
		const float lineHeight = (ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f) * CPStyles::GetUIScale();
		const float buttonWidth = lineHeight + 3.0f;
		float totalWidth = ImGui::GetContentRegionAvail().x - 5.0f;
		float totalSpacing = spacing * 7.0f; // 3 buttons + 3 sliders = 6 elements, 5 spaces between them
		float remainingWidth = totalWidth - ((4 * buttonWidth) + lineHeight) - totalSpacing;
		float xDragWidth = remainingWidth / 4.0f;
		float yDragWidth = (remainingWidth - xDragWidth) / 3.0f;
		float zDragWidth = (remainingWidth - xDragWidth - yDragWidth) / 2.0f;
		float wDragWidth = (remainingWidth - xDragWidth - yDragWidth - zDragWidth);

		ImVec2 startPos = ImGui::GetCursorScreenPos();
		ImVec2 endPos = { startPos.x + totalWidth + 5.0f - lineHeight - spacing, startPos.y + lineHeight };
		drawList->AddRectFilled(startPos, endPos, ImColor(50, 50, 50, 255), rounding);
		ImGui::SetCursorScreenPos(ImVec2(startPos.x, startPos.y));
		ImGui::BeginGroup();

		auto DrawRoundedButton = [&](const char* label, float* value, const ImVec4& color, ImDrawFlags roundFlags)
			{
				ImVec2 btnPos = ImGui::GetCursorScreenPos();
				ImVec2 btnSize = ImVec2(buttonWidth, lineHeight);

				// Handle state
				ImGui::InvisibleButton(label, btnSize);
				bool hovered = ImGui::IsItemHovered();
				bool held = ImGui::IsItemActive();

				ImU32 baseColor = ImGui::GetColorU32(color);
				ImU32 hoveredColor = ImGui::GetColorU32(ImVec4(color.x + 0.1f, color.y + 0.1f, color.z + 0.1f, 1.0f));
				ImU32 activeColor = ImGui::GetColorU32(ImVec4(color.x - 0.1f, color.y - 0.1f, color.z - 0.1f, 1.0f));

				ImU32 drawColor = baseColor;
				if (held) drawColor = activeColor;
				else if (hovered) drawColor = hoveredColor;

				drawList->AddRectFilled(btnPos, ImVec2(btnPos.x + buttonWidth, btnPos.y + lineHeight), drawColor, rounding, roundFlags);

				ImVec2 textSize = ImGui::CalcTextSize(label);
				ImVec2 textPos = ImVec2(btnPos.x + (buttonWidth - textSize.x) * 0.5f, btnPos.y + (lineHeight - textSize.y) * 0.5f);

				drawList->AddText(boldFont, ImGui::GetFontSize(), textPos, ImGui::GetColorU32(ImGuiCol_Text), label);

				if (ImGui::IsItemClicked())
					*value = resetValue;

				ImGui::SameLine(0.0f, spacing);
			};

		auto DrawDrag = [&](const char* id, float* value, float width, int axis)
			{
				ImGui::SetNextItemWidth(width);

				glm::vec4 original = values; // Capture before any changes

				if (ImGui::DragFloat(id, value, 0.1f, 0.0f, 0.0f, "%.2f"))
				{
					edited = true;

					ImGuiID lockID = ImGui::GetID("LockState");
					bool* locked = ImGui::GetStateStorage()->GetBoolRef(lockID, false);

					if (*locked)
					{
						float originalComponent = 0.0f;
						float newComponent = 0.0f;

						switch (axis)
						{
						case 0: originalComponent = original.x; newComponent = values.x; break;
						case 1: originalComponent = original.y; newComponent = values.y; break;
						case 2: originalComponent = original.z; newComponent = values.z; break;
						case 3: originalComponent = original.w; newComponent = values.w; break;
						}

						float ratio = (originalComponent != 0.0f) ? newComponent / originalComponent : 1.0f;

						switch (axis)
						{
						case 0:
							if (original.y != 0.0f) values.y = original.y * ratio;
							if (original.z != 0.0f) values.z = original.z * ratio;
							if (original.w != 0.0f) values.w = original.w * ratio;
							break;
						case 1:
							if (original.x != 0.0f) values.x = original.x * ratio;
							if (original.z != 0.0f) values.z = original.z * ratio;
							if (original.w != 0.0f) values.w = original.w * ratio;
							break;
						case 2:
							if (original.x != 0.0f) values.x = original.x * ratio;
							if (original.y != 0.0f) values.y = original.y * ratio;
							if (original.w != 0.0f) values.w = original.w * ratio;
							break;
						case 3:
							if (original.x != 0.0f) values.x = original.x * ratio;
							if (original.y != 0.0f) values.y = original.y * ratio;
							if (original.z != 0.0f) values.z = original.z * ratio;
							break;
						}
					}
				}

				ImGui::SameLine(0.0f, spacing);
			};


		DrawRoundedButton("X", &values.x, CPStyles::BUTTON_COLOR_X, ImDrawFlags_RoundCornersLeft);
		DrawDrag("##X", &values.x, xDragWidth, 0);
		DrawRoundedButton("Y", &values.y, CPStyles::BUTTON_COLOR_Y, ImDrawFlags_RoundCornersNone);
		DrawDrag("##Y", &values.y, yDragWidth, 1);
		DrawRoundedButton("Z", &values.z, CPStyles::BUTTON_COLOR_Z, ImDrawFlags_RoundCornersNone);
		DrawDrag("##Z", &values.z, zDragWidth, 2);
		DrawRoundedButton("W", &values.w, CPStyles::BUTTON_COLOR_W, ImDrawFlags_RoundCornersNone);
		DrawDrag("##W", &values.w, wDragWidth, 3);

		// Lock button
		ImGui::Spacing();
		ImGui::SameLine();

		ImGui::SetCursorPosX(0.0f);
		ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - lineHeight);
		ImGuiID lockID = ImGui::GetID("LockState");
		bool* locked = ImGui::GetStateStorage()->GetBoolRef(lockID, false);
		ImVec2 lockBtnSize = { lineHeight, lineHeight };
		auto& lockIcon = *locked ? CPResources::ICON_LOCK : CPResources::ICON_UNLOCK;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		if (CPImGui::ImageButton(lockIcon, lockBtnSize))
			*locked = !*locked;
		ImGui::PopStyleColor(3);

		ImGui::EndGroup();
		ImGui::PopStyleVar(2);
		ImGui::Columns(1);
		ImGui::PopID();
		ImGui::Dummy(ImVec2(0, 0.0f));

		return edited;
	}

	bool SliderFloat(const char* label, float* value, float min, float max, float width)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGui::PushID(label);

		const float grabSize = 16.0f;
		const float trackHeight = 4.0f;
		const float totalHeight = grabSize;

		ImVec2 startPos = ImGui::GetCursorScreenPos();

		ImFont* font = ImGui::GetFont();
		float fontSize = font->FontSize;

		char labelWithValue[128];
		snprintf(labelWithValue, sizeof(labelWithValue), "%s: %.3f", label, *value);

		ImVec2 labelSize = ImGui::CalcTextSize(labelWithValue);

		float trackCenterY = startPos.y + totalHeight * 0.5f;
		float labelPosY = trackCenterY - labelSize.y * 0.5f;

		ImDrawList* draw = ImGui::GetWindowDrawList();

		ImVec2 labelPos(startPos.x, labelPosY);
		ImVec2 labelRectMax(labelPos.x + labelSize.x, labelPos.y + labelSize.y);

		ImGui::SetCursorScreenPos(labelPos);

		// ----- Unique editing state -----
		ImGuiID labelID = ImGui::GetID(label); // unique per label
		ImGuiStorage* storage = ImGui::GetStateStorage();
		bool editing = storage->GetBool(labelID, false);

		static char inputBuffer[64];
		std::string inputID = std::string("##editlabel_") + label;

		ImGuiIO& io = ImGui::GetIO();
		bool doubleClicked = false;
		if (ImGui::IsMouseHoveringRect(labelPos, labelRectMax) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			editing = true;
			snprintf(inputBuffer, sizeof(inputBuffer), "%.3f", *value);
			ImGui::SetKeyboardFocusHere();
		}

		bool changed = false;

		if (editing)
		{
			ImGui::SetNextItemWidth(labelSize.x + 50);
			std::string inputID = "##editlabel" + std::string(label);
			if (ImGui::InputText(inputID.c_str(), inputBuffer, sizeof(inputBuffer),
				ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue))
			{
				float newVal = *value;
				if (sscanf(inputBuffer, "%f", &newVal) == 1)
				{
					if (newVal < min) newVal = min;
					if (newVal > max) newVal = max;

					*value = newVal;
					changed = true;
				}
				editing = false;
			}
			else if (!ImGui::IsItemActive() && !ImGui::IsItemHovered())
			{
				editing = false;
			}
		}
		else
		{
			draw->AddText(labelPos, ImGui::GetColorU32(ImGuiCol_Text), labelWithValue);
		}

		storage->SetBool(labelID, editing);

		float spacing = ImGui::GetStyle().ItemSpacing.x;
		ImGui::SetCursorScreenPos(ImVec2(startPos.x + labelSize.x + spacing, startPos.y));

		float sliderWidth = width - labelSize.x - spacing;
		ImGui::SetNextItemWidth(sliderWidth);

		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, IM_COL32(0, 0, 0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 6));
		ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 0.0f);

		std::string sliderID = "##sliderlabel" + std::string(label);
		bool sliderChanged = ImGui::SliderFloat(sliderID.c_str(), value, min, max, " ", ImGuiSliderFlags_NoInput);

		ImVec2 pos = ImGui::GetItemRectMin();
		ImVec2 size = ImGui::GetItemRectSize();

		float trackRadius = trackHeight * 0.5f;
		float trackY = pos.y + (size.y - trackHeight) * 0.5f;
		ImVec2 trackMin(pos.x, trackY);
		ImVec2 trackMax(pos.x + size.x, trackY + trackHeight);
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(trackMin, trackMax, IM_COL32(128, 128, 128, 255), trackRadius);

		float t = (*value - min) / (max - min);
		float sliderUsableX = size.x - grabSize;
		float grabX = pos.x + t * sliderUsableX + grabSize * 0.5f;
		float grabY = pos.y + size.y * 0.5f;

		drawList->AddCircleFilled(ImVec2(grabX, grabY), grabSize * 0.5f, IM_COL32(255, 255, 255, 255), 32);

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(5);
		ImGui::PopID();

		return changed || sliderChanged;
	}

	bool FramedButton(const char* label, const char* buttonLabel, float columnWidth)
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
		ImGui::PushStyleColor(ImGuiCol_Border, CPStyles::ELEMENT_COLOR_BORDER);

		ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

		bool returnValue = ImGui::Button(buttonLabel, ImVec2(ImGui::GetContentRegionAvail().x, CPStyles::GetButtonHeightComponentDragDrop()));
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
		return returnValue;
	}

	bool FramedButtonWithIcon(const char* label, CCRef<Vesuvius::VSTexture2D> icon, const char* buttonLabel, float columnWidth)
	{
		ImGui::PushID(label); // Ensure uniqueness

		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);

		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
		ImGui::PushStyleColor(ImGuiCol_Border, CPStyles::ELEMENT_COLOR_BORDER);

		const float lineHeight = (ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f) * CPStyles::GetUIScale();

		float vertialIconPadding = 2.0f * CPStyles::GetUIScale();
		ImVec2 buttonSize = ImVec2(ImGui::GetContentRegionAvail().x - lineHeight, ImGui::GetTextLineHeight() + vertialIconPadding);
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		ImGui::InvisibleButton("##framedButton", buttonSize);

		bool hovered = ImGui::IsItemHovered();
		bool clicked = ImGui::IsItemClicked();

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImU32 borderColor = ImGui::GetColorU32(ImGuiCol_Border);
		ImU32 bgColor = ImGui::GetColorU32(hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);

		ImVec2 buttonEnd = ImVec2(cursorPos.x + buttonSize.x, cursorPos.y + buttonSize.y);
		drawList->AddRectFilled(cursorPos, buttonEnd, bgColor, CPStyles::GetElementRounding());
		drawList->AddRect(cursorPos, buttonEnd, borderColor, CPStyles::GetElementRounding(), 0, 1.0f);

		// Smaller icon
		const float iconScale = 0.65f;
		float iconHeight = buttonSize.y * iconScale;
		ImVec2 iconSize = ImVec2(iconHeight, iconHeight); // square

		const float spacing = 6.0f;
		ImVec2 textSize = ImGui::CalcTextSize(buttonLabel);
		float totalContentWidth = iconSize.x + spacing + textSize.x;

		drawList->PushClipRect(cursorPos, buttonEnd, true);

		float contentStartX = cursorPos.x + std::max(0.0f, (buttonSize.x - totalContentWidth) * 0.5f);

		ImVec2 iconPos = ImVec2(
			contentStartX,
			cursorPos.y + (buttonSize.y - iconSize.y) * 0.5f
		);

		ImVec2 textPos = ImVec2(
			iconPos.x + iconSize.x + spacing,
			cursorPos.y + (buttonSize.y - textSize.y) * 0.5f
		);

		drawList->AddImage((ImTextureID)(uint64_t)icon->GetRendererID(), iconPos, ImVec2(iconPos.x + iconSize.x, iconPos.y + iconSize.y), ImVec2(0, 1), ImVec2(1, 0));
		drawList->AddText(textPos, textColor, buttonLabel);

		drawList->PopClipRect();

		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);
		ImGui::Columns(1);
		ImGui::PopID();

		return clicked;
	}

	bool FramedCheckbox(const char* label, bool* value, float columnWidth)
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 1 });

		ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

		std::string checkboxID = "##checkboxlabel" + std::string(label);
		bool returnValue = ImGui::Checkbox(checkboxID.c_str(), value);

		ImGui::PopStyleVar(2);
		ImGui::Columns(1);
		return returnValue;
	}

	bool TagCheckbox(bool* value)
	{
		bool returnValue = ImGui::Checkbox("##", value);
		return returnValue;
	}

	bool FramedDragFloat(const char* label, float* value, float speed, float min, float max, const char* format, float columnWidth)
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 1 });

		const float lineHeight = (ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f) * CPStyles::GetUIScale();
		const float spacing = 4.0f;

		ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - lineHeight - spacing);

		std::string sliderID = "##sliderlabel" + std::string(label);
		if (*value < min) *value = min;
		if (*value > max) *value = max;
		bool returnValue = ImGui::DragFloat(sliderID.c_str(), value, speed, min, max, format);
		if (returnValue)
		{
			if (*value < min) *value = min;
			if (*value > max) *value = max;
		}
		ImGui::PopStyleVar(2);
		ImGui::Columns(1);
		return returnValue;
	}

	bool FramedDragInt(const char* label, int* value, float speed, float min, float max, float columnWidth)
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 1 });

		const float lineHeight = (ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f) * CPStyles::GetUIScale();
		const float spacing = 4.0f;

		ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - lineHeight - spacing);

		std::string sliderID = "##sliderlabel" + std::string(label);
		if (*value < min) *value = min;
		if (*value > max) *value = max;
		bool returnValue = ImGui::DragInt(sliderID.c_str(), value, speed, min, max);
		if (returnValue)
		{
			if (*value < min) *value = min;
			if (*value > max) *value = max;
		}
		ImGui::PopStyleVar(2);
		ImGui::Columns(1);
		return returnValue;
	}

	bool ImageButton(CCRef<Vesuvius::VSTexture2D> icon, float size, ImVec4& color)
	{
		return ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), color);
	}

	bool ImageButton(CCRef<Vesuvius::VSTexture2D> icon, ImVec2& size, ImVec4& color)
	{
		return ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), size, ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), color);
	}

	void Image(CCRef<Vesuvius::VSTexture2D> icon, float size, ImVec4& color)
	{
		return ImGui::Image((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), color);
	}

	void Image(CCRef<Vesuvius::VSTexture2D> icon, ImVec2& size, ImVec4& color)
	{
		return ImGui::Image((ImTextureID)(uint64_t)icon->GetRendererID(), size, ImVec2(0, 1), ImVec2(1, 0), color);
	}

	void Image(uint32_t textureID, ImVec2& size, ImVec4& color)
	{
		return ImGui::Image((void*)(intptr_t)textureID, size, { 0, 1 }, { 1, 0 }, color);
	}

	bool MenuItem(const char* label, CCRef<Vesuvius::VSTexture2D> icon, const char* shortcut, bool enabled, bool selected)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 labelSize = ImGui::CalcTextSize(label, nullptr, true);
		ImVec2 shortcutSize = shortcut ? ImGui::CalcTextSize(shortcut) : ImVec2(0, 0);
		float iconSpacing = 6.0f;
		float textPadding = 8.0f;
		ImVec2 iconSize = ImVec2(labelSize.y, labelSize.y);

		float height = ImMax(iconSize.y, labelSize.y) + style.FramePadding.y * 2;
		float width = iconSize.x + iconSpacing + labelSize.x + textPadding + shortcutSize.x + style.FramePadding.x * 2;

		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 size(width, height);

		ImGui::InvisibleButton("##icon_menu_item", size);
		bool hovered = ImGui::IsItemHovered();
		bool clicked = ImGui::IsItemClicked();

		// Draw background highlight
		if (hovered || selected)
		{
			ImU32 bgColor = ImGui::GetColorU32(hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
			ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgColor);
		}

		ImVec2 iconPos = ImVec2(pos.x + style.FramePadding.x, pos.y + (size.y - iconSize.y) * 0.5f);
		ImVec2 labelPos = ImVec2(iconPos.x + iconSize.x + iconSpacing, pos.y + (size.y - labelSize.y) * 0.5f);
		float regionMaxX = ImGui::GetWindowContentRegionMax().x + ImGui::GetWindowPos().x;
		ImVec2 shortcutPos = ImVec2(regionMaxX - style.ItemSpacing.x - shortcutSize.x, pos.y + (size.y - shortcutSize.y) * 0.5f);

		ImU32 textColor = ImGui::GetColorU32(enabled ? ImGuiCol_Text : ImGuiCol_TextDisabled);

		if (icon)
		{
			ImGui::GetWindowDrawList()->AddImage((ImTextureID)(uint64_t)icon->GetRendererID(), iconPos, ImVec2(iconPos.x + iconSize.x, iconPos.y + iconSize.y), ImVec2(0, 1), ImVec2(1, 0));
		}

		ImGui::GetWindowDrawList()->AddText(labelPos, textColor, label);
		if (shortcut && shortcut[0] != '\0')
			ImGui::GetWindowDrawList()->AddText(shortcutPos, ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 0.3f)), shortcut);

		ImGui::PopStyleVar(3);

		if (clicked && enabled)
		{
			ImGui::CloseCurrentPopup();
			return true;
		}

		return false;

		// return ImGui::MenuItem(label, shortcut, selected, enabled);
	}

	bool TreeNode(std::string& id, CCRef<Vesuvius::VSTexture2D> icon, ImGuiTreeNodeFlags flags, const char* label)
	{
		ImGuiContext& context = *ImGui::GetCurrentContext();

		ImVec2 iconSize = CPStyles::GetIconTreeSize();
		const float spacing = 4.0f;

		std::string treeNodeID = std::string("##") + id;

		// Draw tree node arrow only, with empty label to get toggle interaction
		bool open = ImGui::TreeNodeEx(treeNodeID.c_str(), flags);

		// Get bounding box of the last item (arrow area)
		ImVec2 itemMin = ImGui::GetItemRectMin();
		ImVec2 itemMax = ImGui::GetItemRectMax();
		float height = itemMax.y - itemMin.y;

		// Calculate positions
		float indentSpacing = context.Style.IndentSpacing;
		float arrowWidth = context.FontSize;

		ImVec2 iconPos = ImVec2(itemMin.x + indentSpacing + arrowWidth, itemMin.y + (height - iconSize.y) * 0.5f);
		ImVec2 labelPos = ImVec2(iconPos.x + iconSize.x + spacing, itemMin.y + (height - context.FontSize) * 0.5f);

		// Draw icon and label regardless of open state
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddImage(
			(ImTextureID)(uint64_t)icon->GetRendererID(),
			iconPos,
			ImVec2(iconPos.x + iconSize.x, iconPos.y + iconSize.y),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);
		drawList->AddText(labelPos, ImGui::GetColorU32(ImGuiCol_Text), label);

		return open;
	}

	void EntityNode(Entity entity, CCRef<CCScene> context, Entity& selectionContext)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		bool hasChildren = !CCSceneEntityManager::GetChildren(context.get(), entity.GetUUID()).empty();
		if (!hasChildren)
			flags |= ImGuiTreeNodeFlags_Leaf;

		if (!CCSceneEntityManager::IsEnabled(context.get(), entity))
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));

		bool opened = CPImGui::TreeNode(std::to_string((uint64_t)(uint32_t)entity), CPResources::ICON_ENTITY, flags, tag.c_str());

		if (!CCSceneEntityManager::IsEnabled(context.get(), entity))
			ImGui::PopStyleColor();

		if (ImGui::BeginDragDropSource())
		{
			CCUUID id = entity.GetUUID();
			ImGui::SetDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP", &id, sizeof(CCUUID));
			Cataclysm::CCRef<Vesuvius::VSTexture2D> icon = CPResources::ICON_ENTITY;
			CPImGui::Image(icon, CPStyles::GetIconTreeSize());
			ImGui::SameLine();
			ImGui::Text("%s", tag.c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP"))
			{
				CCUUID droppedID = *(const CCUUID*)payload->Data;

				if (droppedID != entity.GetUUID())
				{
					CCSceneEntityManager::SetParent(context.get(), droppedID, entity.GetUUID());
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			selectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			CPImGui::Image(CPResources::ICON_ENTITY, CPStyles::GetIconTreeSize());
			ImGui::SameLine();
			if (ImGui::BeginMenu("New Entity"))
			{
				CPImGui::Image(CPResources::ICON_ENTITY, CPStyles::GetIconTreeSize());
				ImGui::SameLine();
				if (ImGui::MenuItem("Empty Entity"))
				{
					Entity newEntity = CCSceneEntityManager::CreateEntity(context.get(), "Empty Entity");
					CCSceneEntityManager::SetParent(context.get(), newEntity.GetUUID(), entity.GetUUID());
					auto& tc = newEntity.GetComponent<Transform>();
					tc.Translation = { 0.0f, 0.0f, 0.0f };
					tc.Rotation = { 0.0f, 0.0f, 0.0f };
					tc.Scale = { 1.0f, 1.0f, 1.0f };
				}

				CPImGui::Image(CPResources::ICON_AUDIO_SOURCE, CPStyles::GetIconTreeSize(), CPStyles::ICON_COLOR_HIERARCHY);
				ImGui::SameLine();
				if (ImGui::MenuItem("AudioSource"))
				{
					Entity newEntity = CCSceneEntityManager::CreateEntity(context.get(), "Audio Source");
					newEntity.AddComponent<AudioSource>();
					CCSceneEntityManager::SetParent(context.get(), newEntity.GetUUID(), entity.GetUUID());
					auto& tc = newEntity.GetComponent<Transform>();
					tc.Translation = { 0.0f, 0.0f, 0.0f };
					tc.Rotation = { 0.0f, 0.0f, 0.0f };
					tc.Scale = { 1.0f, 1.0f, 1.0f };
				}

				CPImGui::Image(CPResources::ICON_CAMERA, CPStyles::GetIconTreeSize(), CPStyles::ICON_COLOR_HIERARCHY);
				ImGui::SameLine();
				if (ImGui::MenuItem("Camera"))
				{
					Entity newEntity = CCSceneEntityManager::CreateEntity(context.get(), "Camera");
					newEntity.AddComponent<Camera>();
					CCSceneEntityManager::SetParent(context.get(), newEntity.GetUUID(), entity.GetUUID());
					auto& tc = newEntity.GetComponent<Transform>();
					tc.Translation = { 0.0f, 0.0f, 0.0f };
					tc.Rotation = { 0.0f, 0.0f, 0.0f };
					tc.Scale = { 1.0f, 1.0f, 1.0f };
				}

				CPImGui::Image(CPResources::ICON_RIGIDBODY_2D, CPStyles::GetIconTreeSize(), CPStyles::ICON_COLOR_HIERARCHY);
				ImGui::SameLine();
				if (ImGui::MenuItem("Mass"))
				{
					Entity newEntity = CCSceneEntityManager::CreateEntity(context.get(), "Mass");
					newEntity.AddComponent<Rigidbody2D>();
					newEntity.AddComponent<SpriteRenderer>();
					newEntity.AddComponent<BoxCollider2D>();
					CCSceneEntityManager::SetParent(context.get(), newEntity.GetUUID(), entity.GetUUID());
					auto& tc = newEntity.GetComponent<Transform>();
					tc.Translation = { 0.0f, 0.0f, 0.0f };
					tc.Rotation = { 0.0f, 0.0f, 0.0f };
					tc.Scale = { 1.0f, 1.0f, 1.0f };
				}

				CPImGui::Image(CPResources::ICON_SPRITE_RENDERER, CPStyles::GetIconTreeSize(), CPStyles::ICON_COLOR_HIERARCHY);
				ImGui::SameLine();
				if (ImGui::MenuItem("Sprite"))
				{
					Entity newEntity = CCSceneEntityManager::CreateEntity(context.get(), "Sprite");
					newEntity.AddComponent<SpriteRenderer>();
					CCSceneEntityManager::SetParent(context.get(), newEntity.GetUUID(), entity.GetUUID());
					auto& tc = newEntity.GetComponent<Transform>();
					tc.Translation = { 0.0f, 0.0f, 0.0f };
					tc.Rotation = { 0.0f, 0.0f, 0.0f };
					tc.Scale = { 1.0f, 1.0f, 1.0f };
				}

				CPImGui::Image(CPResources::ICON_TEXT, CPStyles::GetIconTreeSize(), CPStyles::ICON_COLOR_HIERARCHY);
				ImGui::SameLine();
				if (ImGui::MenuItem("Text"))
				{
					Entity newEntity = CCSceneEntityManager::CreateEntity(context.get(), "Text");
					newEntity.AddComponent<Text>();
					CCSceneEntityManager::SetParent(context.get(), newEntity.GetUUID(), entity.GetUUID());
					auto& tc = newEntity.GetComponent<Transform>();
					tc.Translation = { 0.0f, 0.0f, 0.0f };
					tc.Rotation = { 0.0f, 0.0f, 0.0f };
					tc.Scale = { 1.0f, 1.0f, 1.0f };
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			CPImGui::Image(CPResources::ICON_DELETE, CPStyles::GetIconTreeSize());
			ImGui::SameLine();
			if (ImGui::MenuItem("Delete"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			for (CCUUID childID : CCSceneEntityManager::GetChildren(context.get(), entity.GetUUID()))
			{
				Entity child = CCSceneEntityManager::GetEntityByUUID(context.get(), childID);
				EntityNode(child, context, selectionContext);
			}

			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			CCSceneEntityManager::DestroyEntity(context.get(), entity);
			if (selectionContext == entity)
				selectionContext = {};
		}
	}
}
