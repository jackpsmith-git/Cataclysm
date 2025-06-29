#pragma once

#include <imgui/imgui.h>

namespace Cataclysm::UI
{
	struct CCImGuiScopedStyleColor
	{
		CCImGuiScopedStyleColor() = default;

		CCImGuiScopedStyleColor(ImGuiCol idx, ImVec4 color, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushStyleColor(idx, color);
		}

		CCImGuiScopedStyleColor(ImGuiCol idx, ImU32 color, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushStyleColor(idx, color);
		}

		~CCImGuiScopedStyleColor()
		{
			if (m_Set)
				ImGui::PopStyleColor();
		}
	private:
		bool m_Set = false;
	};


}
