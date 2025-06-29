#include "ccpch.h"
#include "Cataclysm/Asset/CCAssetMetaSerializer.h"

#include <yaml-cpp/yaml.h>

namespace Cataclysm
{
	void CCAssetMetaSerializer::SerializeMetaFile(const std::filesystem::path& assetPath, CCUUID guid, CCAssetType type)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "ID" << YAML::Value << (uint64_t)guid;
		out << YAML::Key << "Type" << YAML::Value << static_cast<int>(type);

		std::string sub = "Assets\\";
		std::string path = assetPath.string();
		size_t position = path.find(sub);
		if (position != std::string::npos)
		{
			std::string relativePath = path.substr(position + sub.length());
			out << YAML::Key << "FilePath" << YAML::Value << relativePath;
		}
		out << YAML::EndMap;

		std::filesystem::path metaPath = assetPath;
		metaPath += ".meta";

		std::ofstream fout(metaPath);
		fout << out.c_str();
		fout.close();

#ifdef CC_PLATFORM_WINDOWS
		SetFileAttributesW(metaPath.wstring().c_str(), FILE_ATTRIBUTE_HIDDEN);
#endif
	}

	CCAssetMetadata CCAssetMetaSerializer::DeserializeMetaFile(const std::filesystem::path& path)
	{
		CCAssetMetadata metadata;

		try
		{
			YAML::Node root = YAML::LoadFile(path.string());

			uint64_t id = root["ID"].as<uint64_t>();
			metadata.ID = CCUUID(id);
			
			int typeint = root["Type"].as<int>();
			metadata.Type = static_cast<CCAssetType>(typeint);

			if (root["FilePath"])
			{
				std::string filePath = root["FilePath"].as<std::string>();
				std::filesystem::path path = CCProject::GetAssetFileSystemPath(filePath);
				metadata.FilePath = path.string();
			}
		}
		catch (const YAML::Exception& e)
		{
			CC_CORE_WARN("[CCAssetMetaSerializer::DeserializeMetaFile] Failed to deserialize meta file!");
		}

		return metadata;
	}
}
