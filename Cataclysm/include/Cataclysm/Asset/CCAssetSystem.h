#pragma once
#include <filesystem>

#include "Cataclysm.h"
#include "Cataclysm/Vesuvius/VSTexture.h"
#include "Cataclysm/Vesuvius/VSFont.h"
#include "Cataclysm/Audio/CCAudioClip.h"

namespace Cataclysm
{
	enum class CCAssetType
	{
		Unknown,
		Texture2D,
		Font,
		AudioClip
	};

	struct CCAssetMetadata
	{
		CCUUID ID;
		std::string FilePath;
		CCAssetType Type;
	};

	class CCAssetSystem
	{
	public:
		static CCAssetType GetAssetType(const std::filesystem::path& path);
		static void ImportAssets(const std::filesystem::path& assetDirectory);
		static CCUUID GetUUIDFromPath(const std::string& path);

		template<typename T>
		static CCRef<T> GetAsset(CCUUID uuid);

		static void Shutdown();
	};
}
