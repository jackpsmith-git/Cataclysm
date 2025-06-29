#pragma once

#include "Cataclysm.h"
#include "Cataclysm/Scene/CCSceneSerializer.h"
#include "Cataclysm/Scripting/CCScriptEngine.h"

#include "CPResources.h"
#include <Windows.h>
#include <shellapi.h>
#include <regex>
#include <format>

namespace Cataclysm
{
	static class CPFileSystemUtils
	{
	public:
		static std::string GetVS2022InstallPath()
		{
			const char* editions[] = { "Community", "Professional", "Enterprise" };

			for (const char* edition : editions)
			{
				std::ostringstream oss;
				oss << "C:/Program Files/Microsoft Visual Studio/2022/" << edition << "/Common7/IDE/devenv.exe";
				std::filesystem::path path = oss.str();

				if (std::filesystem::exists(path))
					return path.string();
			}

			const char* vsEnv = std::getenv("VSINSTALLDIR");
			if (vsEnv)
			{
				std::filesystem::path path = std::string(vsEnv) + "/Common7/IDE/devenv.exe";
				if (std::filesystem::exists(path))
					return path.string();
			}

			return "";
		}

		static bool OpenScriptInVS(const std::string& solutionPath, const std::string& scriptPath)
		{
			std::string vsPath = GetVS2022InstallPath();
			if (vsPath.empty())
			{
				CC_CORE_ERROR("[CPFileSystemUtils::OpenScriptInVS] Visual Studio 2022 not found in registry!");
				return false;
			}
			std::string arguments = "\"" + solutionPath + "\" /Edit \"" + scriptPath + "\"";

			HINSTANCE result = ShellExecuteA(nullptr, "open", vsPath.c_str(), arguments.c_str(), nullptr, SW_SHOWNORMAL);
			return reinterpret_cast<intptr_t>(result) > 32;
		}

		static void OpenFile(const std::filesystem::path& path, std::filesystem::path& baseDirectory)
		{
			if (path.extension() == ".cs")
			{
				std::string projectName = baseDirectory.parent_path().filename().string();
				std::filesystem::path solutionPath = baseDirectory / "Scripts" / (projectName + ".csproj");
				CPFileSystemUtils::OpenScriptInVS(solutionPath.string(), path.string());
			}
			else if (std::filesystem::is_regular_file(path))
			{
				std::string str = path.generic_string();
				HINSTANCE result = ShellExecuteA(
					NULL,
					"open",
					str.c_str(),
					NULL,
					NULL,
					SW_SHOWNORMAL
				);
			}
		}

		static std::string ParseCSharpClassFullName(const std::filesystem::path& filePath)
		{
			std::ifstream file(filePath);
			if (!file.is_open())
				return "";

			std::string line;
			std::string namespaceName;
			std::string className;

			std::regex namespaceRegex(R"(\s*namespace\s+([a-zA-Z0-9_.]+))");
			std::regex classRegex(R"(\s*(public\s+)?(class|struct)\s+([a-zA-Z0-9_]+))");

			while (std::getline(file, line))
			{
				std::smatch match;
				if (namespaceName.empty() && std::regex_search(line, match, namespaceRegex))
				{
					namespaceName = match[1].str();
				}
				else if (className.empty() && std::regex_search(line, match, classRegex))
				{
					className = match[3].str();
				}

				if (!namespaceName.empty() && !className.empty())
					break;
			}

			file.close();

			if (!className.empty())
			{
				if (!namespaceName.empty())
					return namespaceName + "." + className;
				else
					return className;
			}

			return "";
		}

		static bool IsAudioFile(const std::filesystem::path& path)
		{
			std::string ext = path.extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			return (ext == ".wav" || ext == ".ogg" || ext == ".mp3");
		}

		static bool IsTextureFile(const std::filesystem::path& path)
		{
			std::string ext = path.extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			return (ext == ".png" || ext == ".jpg" || ext == ".jpeg");
		}

		static bool IsCCFile(const std::filesystem::path& path)
		{
			std::string ext = path.extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			return (ext == ".ccscene" || ext == ".ccproject");
		}

		static bool IsCsharpFile(const std::filesystem::path& path)
		{
			std::string ext = path.extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			return ext == ".cs";
		}

		static bool IsFontFile(const std::filesystem::path& path)
		{
			std::string ext = path.extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			return ext == ".ttf";
		}

		static bool IsBatchFile(const std::filesystem::path& path)
		{
			std::string ext = path.extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			return (ext == ".bat");
		}

		static bool IsLuaFile(const std::filesystem::path& path)
		{
			std::string ext = path.extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			return (ext == ".lua");
		}

		static bool IsCSharpProjectFile(const std::filesystem::path& path)
		{
			std::string ext = path.extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			return (ext == ".csproj" || ext == ".user");
		}

		static bool IsSolutionFile(const std::filesystem::path& path)
		{
			std::string ext = path.extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			return (ext == ".sln");
		}

		static bool IsMetaFile(const std::filesystem::path& path)
		{
			std::string ext = path.extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			return (ext == ".meta");
		}

		static bool IsUnwantedFileType(const std::filesystem::path& path)
		{
			return (CPFileSystemUtils::IsBatchFile(path) || CPFileSystemUtils::IsLuaFile(path) || CPFileSystemUtils::IsCSharpProjectFile(path) || CPFileSystemUtils::IsSolutionFile(path) || CPFileSystemUtils::IsMetaFile(path));
		}

		static bool IsUnwantedDirectory(const std::string& path)
		{
			return path == "Binaries" ||
				path == "Intermediates" ||
				path == ".vs";
		}

		static std::filesystem::path FindSolutionFile(const std::filesystem::path& filePath)
		{
			auto dir = filePath.parent_path();

			while (!dir.empty())
			{
				for (const auto& entry : std::filesystem::directory_iterator(dir))
				{
					if (entry.path().extension() == ".sln")
						return entry.path();
				}

				dir = dir.parent_path();
			}

			return {};
		}

		static bool EndsWithFolder(const std::string& path, const std::string& folder)
		{
			std::string normalizedPath = path;
			std::string normalizedFolder = folder;

			while (!normalizedPath.empty() && (normalizedPath.back() == '/' || normalizedPath.back() == '\\'))
			{
				normalizedPath.pop_back();
			}

			if (!normalizedFolder.empty() && normalizedFolder.front() != '/' && normalizedFolder.front() != '\\')
			{
				normalizedFolder = "/" + normalizedFolder;
			}

			return normalizedPath.size() >= normalizedFolder.size() &&
				(normalizedPath.compare(normalizedPath.size() - normalizedFolder.size(), normalizedFolder.size(), normalizedFolder) == 0 ||
					normalizedPath.compare(normalizedPath.size() - normalizedFolder.size(), normalizedFolder.size(), "\\" + folder) == 0);
		}

		static CCRef<Vesuvius::VSTexture2D> GetFileThumbnail(const std::filesystem::directory_entry& directoryEntry, std::unordered_map<std::string, CCRef<Vesuvius::VSTexture2D>>& thumbnailCache)
		{
			Cataclysm::CCRef<Vesuvius::VSTexture2D> icon = directoryEntry.is_directory() ? CPResources::ICON_DIRECTORY : CPResources::ICON_FILE;

			auto& path = directoryEntry.path();
			
			if (IsTextureFile(path))
			{
				std::string fullPath = path.string();
				if (thumbnailCache.find(fullPath) == thumbnailCache.end())
				{
					CCRef<Vesuvius::VSTexture2D> texture = Vesuvius::VSTexture2D::Create(fullPath);
					if (texture && texture->IsLoaded())
						thumbnailCache[fullPath] = texture;
					else
						thumbnailCache[fullPath] = CPResources::ICON_FILE;
				}

				icon = thumbnailCache[fullPath];
			}
			else if (IsCCFile(path))
			{
				icon = CPResources::ICON_CATACLYSM_WHITE;
			}
			else if (IsCsharpFile(path))
			{
				icon = CPResources::ICON_CSHARP_LOGO_320;
			}
			else if (IsFontFile(path))
			{
				icon = CPResources::ICON_FONT;
			}
			else if (IsAudioFile(path))
			{
				icon = CPResources::ICON_AUDIO_CLIP;
			}

			return icon;
		}
	
		static void CreateMonoScript(std::string& scriptName, std::filesystem::path& baseDirectory, std::filesystem::path& currentDirectory)
		{
			std::string templateScript =
R"(using System;

using Cataclysm;
using Cataclysm.Input;
using Cataclysm.Math;

namespace ProjectName
{
	public class NewClass : Entity
	{
		public void OnCreate()
		{
			
		}	
	
		public void OnUpdate(float ts)
		{
			
		}
	}
}
)";

			CPFileSystemUtils::ReplaceClassWithFileName(scriptName, templateScript);

			std::string projectName = baseDirectory.parent_path().filename().string();
			CPFileSystemUtils::ReplaceNamespaceWithProjectName(projectName, templateScript);

			std::ofstream out(currentDirectory / (scriptName + ".cs"));
			if (out.is_open())
			{
				out << templateScript;
				out.close();
			}

			std::filesystem::path csprojPath = std::filesystem::path("Scripts") / (projectName + ".cs");				
			std::ifstream inFile(csprojPath);
			std::string csprojContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
			inFile.close();

			// Add script to project

			std::string newScriptPath = "Scripts/" + scriptName + ".cs";

			if (csprojContent.find(newScriptPath) == std::string::npos)
			{
				const std::string tag = "</ItemGroup>";
				std::string compileTag = "    <Compile Include=\"" + newScriptPath + "\" />\n";

				size_t pos = csprojContent.find(tag);
				if (pos != std::string::npos)
					csprojContent.insert(pos, compileTag);

				std::ofstream outFile(csprojPath);
				outFile << csprojContent;
				outFile.close();
			}

			CCScriptEngine::ReloadAssembly();

			// Open Script
			std::filesystem::path solutionPath = baseDirectory / "Scripts" / (projectName + ".csproj");
			std::filesystem::path absoluteScriptPath = baseDirectory / "Scripts" / "Source" / (scriptName + ".cs");
			CPFileSystemUtils::OpenScriptInVS(solutionPath.string(), absoluteScriptPath.string());
		}

		static void ReplaceNamespaceWithProjectName(std::string& projectName, std::string& templateScript)
		{
			size_t pos = templateScript.find("ProjectName");
			if (pos != std::string::npos)
				templateScript.replace(pos, strlen("ProjectName"), projectName);
		}

		static void ReplaceClassWithFileName(std::string& fileName, std::string& templateScript)
		{
			size_t pos = templateScript.find("NewClass");
			if (pos != std::string::npos)
				templateScript.replace(pos, strlen("NewClass"), fileName);
		}

		static void ReplaceAll(std::string& str, const std::string& from, const std::string& to)
		{
			if (from.empty())
				return;
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != std::string::npos)
			{
				str.replace(start_pos, from.length(), to);
				start_pos += to.length();
			}
		}

		static void RunBatch(const std::filesystem::path& batchFilePath)
		{
			std::string batchDir = batchFilePath.parent_path().string();
			std::string batchFile = batchFilePath.filename().string();

			std::string command = "cmd /c \"cd /d \"" + batchDir + "\" && \"" + batchFile + "\"\"";

			int result = system(command.c_str());
			if (result != 0)
				std::cerr << "Batch failed with exit code " << result << "\n";
		}
	};

	static class CPSceneUtils
	{
	public:
		static void SerializeScene(Cataclysm::CCRef<Cataclysm::CCScene> scene, const std::filesystem::path& path)
		{
			Cataclysm::CCSceneSerializer serializer(scene);
			serializer.Serialize(path.string());
		}
	};
}
