#include "ccpch.h"
#include "ImGui/CPImGuiPanel.h"

#include "CPStyles.h"

namespace Cataclysm::CPImGui
{
	void BeginPanel(const char* name)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(name);
	}

	void EndPanel()
	{
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void PanelBackground()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window)
		{
			ImVec2 pos = window->Pos;
			ImVec2 size = window->Size;

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(0, 0, 0, 255));
		}

		ImGui::SetCursorPosX(0);

		ImGui::PopStyleVar();
	}

	void BeginPanelMenu()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window)
		{
			ImVec2 pos = window->Pos;
			ImVec2 size = window->Size;

			ImDrawList* drawList = ImGui::GetWindowDrawList();

			ImVec2 contentRegionMin = ImGui::GetCursorScreenPos();
			ImVec2 contentRegionMax = ImVec2(contentRegionMin.x + ImGui::GetContentRegionAvail().x, contentRegionMin.y + CPStyles::GetPanelMenuHeight());
			drawList->AddRectFilled(contentRegionMin, contentRegionMax, CPStyles::PANEL_MENU_COLOR_BACKGROUND);
		}

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

		ImGui::SetCursorPosX(0);
	}

	void EndPanelMenu()
	{
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(4);
	}

	void BeginPanelContent()
	{
		ImGui::Spacing();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
		ImGui::BeginChild("Content", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
	}

	void EndPanelContent()
	{
		ImGui::EndChild();
		ImGui::PopStyleVar();
	}
}
