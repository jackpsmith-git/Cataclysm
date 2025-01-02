#pragma once

#include <string>
#include <filesystem>

#include <Cataclysm.h>
#include "Cataclysm/Core/Core.h"

namespace Cataclysm 
{
	struct ProjectConfig
	{
		std::string Name = "Untitled";
		std::filesystem::path StartScene;
		std::filesystem::path AssetDirectory;
		std::filesystem::path ScriptModulePath;
	};

	class Project
	{
	public:
		static const std::filesystem::path& GetProjectDirectory()
		{
			CC_CORE_ASSERT(s_ActiveProject, "Project not found!");
			return s_ActiveProject->m_ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory()
		{
			CC_CORE_ASSERT(s_ActiveProject, "Project not found!");
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path)
		{
			CC_CORE_ASSERT(s_ActiveProject, "Project not found!");
			return GetAssetDirectory() / path;
		}

		ProjectConfig& GetConfig() { return m_Config; }
		static Ref<Project> GetActive() { return s_ActiveProject; }
		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);
	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;
		inline static Ref<Project> s_ActiveProject;
	};
}
