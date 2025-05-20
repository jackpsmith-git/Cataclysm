#include "ccpch.h"
#include "SettingsPanel.h"

#include <Cataclysm.h>
#include <imgui/imgui.h>

namespace Cataclysm
{
	SettingsPanel::SettingsPanel()
	{

	}

	void SettingsPanel::OnImGuiRender()
	{
		ImGui::Begin("Settings");



		ImGui::End();
	}
}
