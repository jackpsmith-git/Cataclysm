#include "ccpch.h"
#include "Panels/CPWelcomePanel.h"

#include <Cataclysm.h>
#include <imgui/imgui.h>

namespace Cataclysm
{
	CPWelcomePanel::CPWelcomePanel()
	{

	}

	void CPWelcomePanel::OnImGuiRender(bool& panelEnabled)
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
