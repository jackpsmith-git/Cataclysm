#pragma once
#include <filesystem>
#include "CCAssetSystem.h"
#include "Cataclysm.h"

namespace Cataclysm
{
	class CCAssetMetaSerializer
	{
	public:
		static void SerializeMetaFile(const std::filesystem::path& assetPath, CCUUID guid, CCAssetType type);
		static CCAssetMetadata DeserializeMetaFile(const std::filesystem::path& path);
	};
}
