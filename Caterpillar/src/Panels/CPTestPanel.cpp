#include "ccpch.h"
#include "Panels/CPTestPanel.h"

#include <imgui/imgui.h>

#include "ImGui\CPImGui.h"

namespace Cataclysm
{
	CPTestPanel::CPTestPanel()
	{

	}

	void CPTestPanel::OnImGuiRender()
	{
		CPImGui::BeginPanel("Test");

		CPImGui::BeginPanelMenu();
		CPImGui::EndPanelMenu();

		CPImGui::BeginPanelContent();
		{
			float value = m_SliderValue;
			float width = ImGui::GetContentRegionAvail().x;
			if (CPImGui::SliderFloat("TestSliderFloat", &value, 0.5f, 2.0f, width))
			{
				m_SliderValue = value;
			}
		}
		CPImGui::EndPanelContent();

		CPImGui::EndPanel();
	}
}
