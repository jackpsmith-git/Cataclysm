#include "ccpch.h"
#include "Cataclysm/Project/CCProjectSerializer.h"

#include "Cataclysm.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace Cataclysm 
{
	CCProjectSerializer::CCProjectSerializer(CCRef<CCProject> project)
		: m_Project(project)
	{
	}

	bool CCProjectSerializer::Serialize(const std::filesystem::path& filepath)
	{
		CC_CORE_WARN("[CCProjectSerializer::Serialize] Saving in progress. Do not close app or edit project data!");

		const auto& config = m_Project->GetConfig();
		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "Project" << YAML::Value;
			{
				out << YAML::BeginMap;// Project
				out << YAML::Key << "Name" << YAML::Value << config.Name;
				out << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
				out << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectory.string();
				out << YAML::Key << "ScriptModulePath" << YAML::Value << config.ScriptModulePath.string();
				out << YAML::Key << "PixelDensity" << YAML::Value << config.PixelDensity;
				out << YAML::EndMap; // Project
			}
			out << YAML::EndMap; // Root
		}
		std::ofstream fout(filepath);
		fout << out.c_str();

		CC_CORE_TRACE("[CCProjectSerializer::Serialize] Project saved!");
		return true;
	}

	bool CCProjectSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		auto& config = m_Project->GetConfig();
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::ParserException e)
		{
			CC_CORE_ERROR("[CCProjectSerializer::Deserialize] Failed to load project file!");
			CC_CORE_ERROR(filepath.string());
			return false;
		}
		auto projectNode = data["Project"];

		if (!projectNode)
		{
			CC_CORE_ERROR("[CCProjectSerializer::Deserialize] No project node found!");
			return false;
		}

		config.Name = projectNode["Name"].as<std::string>();
		config.StartScene = projectNode["StartScene"].as<std::string>();
		config.AssetDirectory = projectNode["AssetDirectory"].as<std::string>();
		config.ScriptModulePath = projectNode["ScriptModulePath"].as<std::string>();
		config.PixelDensity = projectNode["PixelDensity"].as<float>(100.0f);
		return true;
	}
}
