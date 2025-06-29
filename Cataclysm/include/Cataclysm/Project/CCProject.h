#pragma once

#include <string>
#include <filesystem>

#include <Cataclysm.h>
#include "Cataclysm/Core/CCCore.h"

namespace Cataclysm 
{
	struct CCProjectConfig
	{
		std::string Name = "Untitled";
		std::filesystem::path StartScene;
		std::filesystem::path AssetDirectory;
		std::filesystem::path ScriptModulePath;
		float PixelDensity = 100.0f;
	};

	class CCProject
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

		CCProjectConfig& GetConfig() { return m_Config; }
		static CCRef<CCProject> GetActive() { return s_ActiveProject; }
		static CCRef<CCProject> New();
		static CCRef<CCProject> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);
	private:
		CCProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;
		inline static CCRef<CCProject> s_ActiveProject;
	};
}
