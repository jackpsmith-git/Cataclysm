#pragma once

#include <imgui/imgui.h>

namespace Cataclysm
{
	class CPStyles
	{
	private:
		inline static float m_GlobalScale = 1.0f;
		inline static float m_StandardSize = 18.0f;

		inline static float m_ButtonVerticalPaddingComponentDragDrop = 2.0f;

		inline static float m_ThumbnailSizeSmall = 50.0f;
		inline static float m_ThumbnailSizeLarge = 80.0f;

		inline static ImVec2 m_ButtonSizeDialog = ImVec2(120, 0);

		inline static float m_ElementRounding = 4.0f;
		inline static float m_VectorControlSpacing = 4.0f;
		inline static float m_VectorControlFramePadding = 4.0f;
	public:
		// COLOR
		inline static ImU32 PANEL_MENU_COLOR_BACKGROUND = IM_COL32(50, 50, 50, 255);
		
		inline static ImVec4& ICON_COLOR_HIERARCHY = ImVec4(1, 1, 1, 1);			// White
		inline static ImVec4& ICON_COLOR_EDITOR_TOOLBAR = ImVec4(1, 1, 1, 1);		// White

		inline static ImVec4& TEXT_COLOR_OUTPUT_WARNING = ImVec4(1, 1, 0, 1);		// Yellow
		inline static ImVec4& TEXT_COLOR_OUTPUT_ERROR = ImVec4(1, 0, 0, 1);			// Red
		inline static ImVec4& TEXT_COLOR_OUTPUT_TRACE = ImVec4(0, 1, 0, 1);			// Green
		inline static ImVec4& TEXT_COLOR_OUTPUT_INFO = ImVec4(1, 1, 1, 1);			// White
		inline static ImVec4& TEXT_COLOR_OUTPUT_DEFAULT = ImVec4(1, 1, 1, 1);		// White
		inline static ImU32 ELEMENT_COLOR_BORDER = IM_COL32(120, 120, 120, 255);
		inline static ImVec4& BUTTON_COLOR_X = ImVec4(0.8f, 0.1f, 0.15f, 1.0f);
		inline static ImVec4& BUTTON_COLOR_Y = ImVec4(0.2f, 0.7f, 0.3f, 1.0f);
		inline static ImVec4& BUTTON_COLOR_Z = ImVec4(0.2f, 0.5f, 0.9f, 1.0f);
		inline static ImVec4& BUTTON_COLOR_W = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

		// SCALE
		inline static float GetUIScale() { return m_GlobalScale; }
		inline static float GetStandardSize() { return m_StandardSize * m_GlobalScale; }
		inline static float GetPanelMenuHeight() { return m_StandardSize * m_GlobalScale; }
		inline static float GetStatusBarHeight() { return m_StandardSize * m_GlobalScale; }
		inline static ImVec2 GetIconSize() { return ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight()); }
		inline static ImVec2 GetIconTreeSize() { return ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight()); }
		inline static float GetButtonHeightComponentDragDrop() { return ((GetStandardSize() * m_GlobalScale)); }
		inline static float GetThumbnailSizeSmall() { return m_ThumbnailSizeSmall * m_GlobalScale; }
		inline static float GetThumbnailSizeLarge() { return m_ThumbnailSizeLarge * m_GlobalScale; }
		inline static ImVec2 GetButtonSizeDialog() { return ImVec2(m_ButtonSizeDialog.x * m_GlobalScale, m_ButtonSizeDialog.y * m_GlobalScale); }
		inline static float GetElementRounding() { return m_ElementRounding; }
		inline static float GetVectorControlSpacing() { return m_VectorControlSpacing; }
		inline static float GetVectorControlFramePadding() { return m_VectorControlFramePadding; }

		inline static void SetUIScale(float scale)
		{
			m_GlobalScale = scale;
			auto& io = ImGui::GetIO();
			io.FontGlobalScale = m_GlobalScale;
		}

	};
}
