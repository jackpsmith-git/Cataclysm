#include "ccpch.h"
#include "Cataclysm/Asset/CCAssetSystem.h"

#include "Cataclysm/Asset/CCAssetMetaSerializer.h"
#include "Cataclysm/Audio/CCAudioEngine.h"

#include "Cataclysm.h"

namespace Cataclysm
{
	static std::unordered_map<std::string, CCUUID> s_FilePathIDMap;
	static std::unordered_map<CCUUID, CCAssetMetadata> s_MetadataMap;
	static std::unordered_map<CCUUID, CCRef<Vesuvius::VSTexture2D>> s_TextureMap;
	static std::unordered_map<CCUUID, CCRef<Vesuvius::VSFont>> s_FontMap;
	static std::unordered_map<CCUUID, CCRef<CCAudioClip>> s_AudioClipMap;

	CCAssetType CCAssetSystem::GetAssetType(const std::filesystem::path& path)
	{
		const auto ext = path.extension().string();
		if (ext == ".png" || ext == ".jpg" || ext == ".jpeg")	return CCAssetType::Texture2D;
		if (ext == ".ttf" || ext == ".otf")						return CCAssetType::Font;
		if (ext == ".wav" || ext == ".ogg" || ext == ".mp3")	return CCAssetType::AudioClip;
																return CCAssetType::Unknown;
	}

	template<>
	CCRef<Vesuvius::VSTexture2D> CCAssetSystem::GetAsset(CCUUID uuid)
	{
		auto& meta = s_MetadataMap.at(uuid);

		auto it = s_TextureMap.find(uuid);
		if (it != s_TextureMap.end())
			return it->second;

		CCRef<Vesuvius::VSTexture2D> texture = Vesuvius::VSTexture2D::Create(meta.FilePath);
		s_TextureMap[uuid] = texture;
		return texture;
	}

	template<>
	CCRef<Vesuvius::VSFont> CCAssetSystem::GetAsset(CCUUID uuid)
	{
		auto& meta = s_MetadataMap.at(uuid);

		auto it = s_FontMap.find(uuid);
		if (it != s_FontMap.end())
			return it->second;

		CCRef<Vesuvius::VSFont> font = CreateRef<Vesuvius::VSFont>(meta.FilePath);
		s_FontMap[uuid] = font;
		return font;
	}

	template<>
	CCRef<CCAudioClip> CCAssetSystem::GetAsset(CCUUID uuid)
	{
		auto& meta = s_MetadataMap.at(uuid);

		auto it = s_AudioClipMap.find(uuid);
		if (it != s_AudioClipMap.end())
			return it->second;

		CCRef<CCAudioClip> audioClip = CCAudioEngine::LoadClip(meta.FilePath);
		s_AudioClipMap[uuid] = audioClip;
		return audioClip;
	}

	void CCAssetSystem::ImportAssets(const std::filesystem::path& assetDirectory)
	{
		s_MetadataMap.clear();

		for (auto& entry : std::filesystem::recursive_directory_iterator(assetDirectory))
		{
			if (!entry.is_regular_file())
				continue;

			auto& path = entry.path();

			if (path.extension() == ".meta")
				continue;

			CCAssetType type = GetAssetType(path);
			if (type == CCAssetType::Unknown)
				continue;

			std::filesystem::path metaPath = path.string() + ".meta";
			if (!std::filesystem::exists(metaPath))
			{
				CCAssetMetaSerializer::SerializeMetaFile(path, CCUUID(), type);
			}

			CCAssetMetadata metadata = CCAssetMetaSerializer::DeserializeMetaFile(metaPath);
			s_MetadataMap[metadata.ID] = metadata;
			s_FilePathIDMap[metadata.FilePath] = metadata.ID;

			switch (type)
			{
			case CCAssetType::Texture2D:
				GetAsset<Vesuvius::VSTexture2D>(metadata.ID);
				break;
			case CCAssetType::AudioClip:
				GetAsset<CCAudioClip>(metadata.ID);
				break;
			case CCAssetType::Font:
				GetAsset<Vesuvius::VSFont>(metadata.ID);
				break;
			}
		}
	}

	void CCAssetSystem::Shutdown()
	{
		s_FilePathIDMap.clear();
		s_MetadataMap.clear();
		s_TextureMap.clear();
		s_FontMap.clear();
		s_AudioClipMap.clear();
	}

	CCUUID CCAssetSystem::GetUUIDFromPath(const std::string& path)
	{
		auto it = s_FilePathIDMap.find(path);
		if (it != s_FilePathIDMap.end())
			return it->second;

		return CCUUID();
	}
}
