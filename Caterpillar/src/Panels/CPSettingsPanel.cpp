#include "ccpch.h"
#include "Panels/CPSettingsPanel.h"

#include "ImGui/CPImGui.h"

#include <Cataclysm.h>
#include <imgui/imgui.h>

namespace Cataclysm
{
	CPSettingsPanel::CPSettingsPanel()
	{

	}

	void CPSettingsPanel::OnImGuiRender(const CCRef<CCScene>& scene)
	{
		CPImGui::BeginPanel("Settings");

		//CPImGui::BeginPanelMenu();
		//CPImGui::EndPanelMenu();

		CPImGui::BeginPanelContent();
		// ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::BeginTabBar("SettingsPanels"))
		{
			if (ImGui::BeginTabItem("Project"))
			{
				// Project settings go here
				CPImGui::FramedDragFloat("Pixel Density", &CCProject::GetActive()->GetConfig().PixelDensity, 1.0f, 1.0f, FLT_MAX, "%.1f", ImGui::GetContentRegionAvail().x * 0.4f);

				ImGui::EndTabItem();
			}

			//if (ImGui::BeginTabItem("Scene"))
			//{
			//	// Scene settings go here
			//	float color[4] = { scene->SkyboxColor.r, scene->SkyboxColor.g, scene->SkyboxColor.b, scene->SkyboxColor.a };
			//	if (CPImGui::ColorSelect("Skybox Color", color, ImGui::GetContentRegionAvail().x * 0.4f))
			//	{
			//		scene->SkyboxColor = glm::vec4(color[0], color[1], color[2], color[3]);
			//	}

			//	ImGui::EndTabItem();
			//}
			ImGui::EndTabBar();
		}

		CPImGui::EndPanelContent();

		CPImGui::EndPanel();
	}
}
