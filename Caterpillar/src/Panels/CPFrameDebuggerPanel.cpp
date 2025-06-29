#include "ccpch.h"
#include "Panels/CPFrameDebuggerPanel.h"

#include "Cataclysm/Core/CCTimestep.h"
#include <imgui/imgui.h>

namespace Cataclysm
{
	void CPFrameDebuggerPanel::OnImGuiRender(CCTimestep ts)
	{
		ImGui::Begin("Frame");
		float sample = 1 / (ts.GetSeconds());
		samples[0] = sample;

		for (int n = 99; n > 0; n--)
			samples[n] = samples[n - 1];

		float min = *std::min_element(samples, samples + sizeof(samples) / sizeof(samples[0]));
		float max = *std::max_element(samples, samples + sizeof(samples) / sizeof(samples[0]));

		ImGui::PlotLines("", samples, 100, 0, "", 0, 300, ImVec2(ImGui::GetContentRegionAvail().x, 100));

		ImGui::PushItemWidth(20.0f);
		ImGui::Text("Framerate:");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Text("%.2f", 1.0f / ts.GetSeconds());

		ImGui::PushItemWidth(20.0f);
		ImGui::Text("Frametime:");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::Text("%.2f", ts.GetMilliseconds());

		ImGui::End();
	}
}
