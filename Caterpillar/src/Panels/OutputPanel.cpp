#include "ccpch.h"
#include "OutputPanel.h"

#include <imgui/imgui.h>

namespace Cataclysm
{
	void OutputPanel::OnImGuiRender()
	{
		ImGui::Begin("Output");

		ImGui::BeginChild("Scrolling");

		// TODO:: Read from log file and display in output panel
		std::ifstream file("Cataclysm.log");

		if (!file.is_open())
		{
			std::cerr << "Error opening file!" << std::endl;
			return;
		}

		std::string line;
		while (std::getline(file, line))
		{
			if (line.find("warning") != std::string::npos)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, {1.0f, 1.0f, 0.0f, 1.0f}); // Yellow
				
				std::string substr = "[warning] ";
				size_t pos = line.find(substr);

				line.erase(pos, substr.length());
			}
			else if (line.find("error") != std::string::npos)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 0.0f, 1.0f }); // Red

				std::string substr = "[error] ";
				size_t pos = line.find(substr);

				line.erase(pos, substr.length());
			}
			else if (line.find("trace") != std::string::npos)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 1.0f }); // White

				std::string substr = "[trace] ";
				size_t pos = line.find(substr);

				line.erase(pos, substr.length());
			}
			else if (line.find("info") != std::string::npos)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 1.0f, 0.0f, 1.0f }); // Green

				std::string substr = "[info] ";
				size_t pos = line.find(substr);

				line.erase(pos, substr.length());
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 1.0f }); // White
			}

			//ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 1.0f });

			ImGui::Text(line.c_str());
			ImGui::PopStyleColor();
		}

		file.close();

		ImGui::EndChild();

		ImGui::End();
	}

	void OutputPanel::ParseMessageType(std::string message, std::string type)
	{
	
	}
}
