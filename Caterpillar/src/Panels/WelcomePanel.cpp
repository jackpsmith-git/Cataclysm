#include "ccpch.h"
#include "WelcomePanel.h"

#include <Cataclysm.h>
#include <imgui/imgui.h>

namespace Cataclysm
{
	WelcomePanel::WelcomePanel()
	{

	}

	void WelcomePanel::OnImGuiRender(bool& panelEnabled)
	{
		ImGui::Begin("Welcome");

		ImGui::Text("Welcome to Caterpillar!");

		if (ImGui::Button("Close", ImVec2(100, 30)))
		{
			panelEnabled = false;
		}

		ImGui::End();
	}
}
