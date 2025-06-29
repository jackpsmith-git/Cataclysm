#include "ccpch.h"
#include "Panels/CPVesuviusPanel.h"

#include <imgui/imgui.h>

#include "ImGui\CPImGui.h"

namespace Cataclysm
{
	CPVesuviusPanel::CPVesuviusPanel()
	{

	}

	void CPVesuviusPanel::OnImGuiRender()
	{
		ImGui::Begin("Vesuvius");

		auto stats = Vesuvius::VSRenderer2D::GetStats();
		ImGui::Text("Draw Calls:  %d", stats.DrawCalls);
		ImGui::Text("Quads:	   %d", stats.QuadCount);
		ImGui::Text("Vertices:    %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices:     %d", stats.GetTotalIndexCount());

		ImGui::End();
	}
}
