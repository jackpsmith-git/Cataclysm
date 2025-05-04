#include "ccpch.h"
#include "FrameDebuggerPanel.h"

#include "Cataclysm/Core/Timestep.h"
#include <imgui/imgui.h>

namespace Cataclysm
{
	void FrameDebuggerPanel::OnImGuiRender(Timestep ts)
	{
		ImGui::Begin("Frame Debugger");
		
		float sample = ts.GetMilliseconds();
		samples[0] = sample;

		for (int n = 99; n > 0; n--)
			samples[n] = samples[n - 1];

		float min = *std::min_element(samples, samples + sizeof(samples) / sizeof(samples[0]));
		float max = *std::max_element(samples, samples + sizeof(samples) / sizeof(samples[0]));

		ImGui::PlotLines("Frametime", samples, 100, 0, "", min, max);

		ImGui::Text("Frametime:");
		ImGui::SameLine();
		ImGui::LabelText(std::to_string(ts.GetMilliseconds()).c_str(), "%0.00"); // Frametime

		ImGui::Text("Framerate:");
		ImGui::SameLine();
		ImGui::LabelText(std::to_string(1 / ts.GetSeconds()).c_str(), "%0"); // Framerate

		ImGui::End();
	}
}
