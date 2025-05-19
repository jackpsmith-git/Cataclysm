#pragma once

#include <filesystem>

#include "Cataclysm/Core/Core.h"
#include "Cataclysm/Renderer/Texture.h"

namespace Cataclysm 
{
	struct MSDFData;

	class Font
	{
	public:
		Font(const std::filesystem::path& font);
		~Font();

		const MSDFData* GetMSDFData() const { return m_Data; }
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static Ref<Font> GetDefault();

		static Ref<Font> GetFont(const std::string& path)
		{
			if (m_FontCache.find(path) != m_FontCache.end())
				return m_FontCache[path];

			Ref<Font> font = CreateRef<Font>(path);
			m_FontCache[path] = font;
			return font;
		}
	private:
		MSDFData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
		static inline std::unordered_map<std::string, Ref<Font>> m_FontCache;
	};
}
