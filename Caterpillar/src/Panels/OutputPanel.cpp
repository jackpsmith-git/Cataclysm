#include "ccpch.h"
#include "OutputPanel.h"

#include <imgui/imgui.h>

namespace Cataclysm
{
	void OutputPanel::OnImGuiRender(Cataclysm::Ref<Cataclysm::Texture2D> clearIcon,
		Cataclysm::Ref<Cataclysm::Texture2D> errorIcon,
		Cataclysm::Ref<Cataclysm::Texture2D> warningIcon,
		Cataclysm::Ref<Cataclysm::Texture2D> traceIcon,
		Cataclysm::Ref<Cataclysm::Texture2D> infoIcon,
		Cataclysm::Ref<Cataclysm::Texture2D> errorIconFaded,
		Cataclysm::Ref<Cataclysm::Texture2D> warningIconFaded,
		Cataclysm::Ref<Cataclysm::Texture2D> traceIconFaded,
		Cataclysm::Ref<Cataclysm::Texture2D> infoIconFaded)
	{
		ImGui::Begin("Output");

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));
		
		float size = 21.0f;
		ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - ((size * 4) + 20));

		if (ImGui::ImageButton((ImTextureID)(uint64_t)clearIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
		{
			// Clear log file
			std::ofstream file("Cataclysm.log", std::ios::out);
			if (file.is_open())
			{
				file.close();			
			}			
		}

		ImGui::SameLine();
		ImGui::Spacing();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		ImGui::SameLine();
		if (errorsEnabled)
		{
			if (ImGui::ImageButton((ImTextureID)(uint64_t)errorIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
			{
				errorsEnabled = false;
			}
		}
		else
		{
			if (ImGui::ImageButton((ImTextureID)(uint64_t)errorIconFaded->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
			{
				errorsEnabled = true;
			}
		}

		ImGui::SameLine();
		if (warningsEnabled)
		{
			if (ImGui::ImageButton((ImTextureID)(uint64_t)warningIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
			{
				warningsEnabled = false;
			}
		}
		else
		{
			if (ImGui::ImageButton((ImTextureID)(uint64_t)warningIconFaded->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1,0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
			{
				warningsEnabled = true;
			}
		}

		ImGui::SameLine();
		if (traceEnabled)
		{
			if (ImGui::ImageButton((ImTextureID)(uint64_t)traceIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
			{
				traceEnabled = false;
			}
		}
		else
		{
			if (ImGui::ImageButton((ImTextureID)(uint64_t)traceIconFaded->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
			{
				traceEnabled = true;
			}
		}

		ImGui::SameLine();
		if (infoEnabled)
		{
			if (ImGui::ImageButton((ImTextureID)(uint64_t)infoIcon->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
			{
				infoEnabled = false;
			}
		}
		else
		{
			if (ImGui::ImageButton((ImTextureID)(uint64_t)infoIconFaded->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
			{
				infoEnabled = true;
			}
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar();

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
				
				if (warningsEnabled)
				{
					std::string substr = "[warning] ";
					size_t pos = line.find(substr);

					line.erase(pos, substr.length());

					ImGui::TextWrapped(line.c_str());
				}
			}
			else if (line.find("error") != std::string::npos)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 0.0f, 1.0f }); // Red

				if (errorsEnabled)
				{
					std::string substr = "[error] ";
					size_t pos = line.find(substr);

					line.erase(pos, substr.length());

					ImGui::TextWrapped(line.c_str());
				}
			}
			else if (line.find("trace") != std::string::npos)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 1.0f }); // White

				if (traceEnabled)
				{
					std::string substr = "[trace] ";
					size_t pos = line.find(substr);

					line.erase(pos, substr.length());

					ImGui::TextWrapped(line.c_str());
				}
			}
			else if (line.find("info") != std::string::npos)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 1.0f, 0.0f, 1.0f }); // Green

				if (infoEnabled)
				{
					std::string substr = "[info] ";
					size_t pos = line.find(substr);

					line.erase(pos, substr.length());

					ImGui::TextWrapped(line.c_str());
				}
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 1.0f }); // White

				ImGui::TextWrapped(line.c_str());
			}

			//ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 1.0f, 1.0f });

			// ImGui::Text(line.c_str());
			// ImGui::SetScrollY(ImGui::GetScrollMaxY());
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
