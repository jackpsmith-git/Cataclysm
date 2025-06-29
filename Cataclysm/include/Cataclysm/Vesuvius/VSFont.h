#pragma once

#include <filesystem>

#include "Cataclysm/Core/CCCore.h"
#include "Cataclysm/Vesuvius/VSTexture.h"

namespace Cataclysm::Vesuvius 
{
	struct VSMSDFData;

	class VSFont
	{
	public:
		VSFont(const std::filesystem::path& font);
		~VSFont();

		const VSMSDFData* GetMSDFData() const { return m_Data; }
		CCRef<VSTexture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static CCRef<VSFont> GetDefault();

		static CCRef<VSFont> GetFont(const std::string& path)
		{
			if (m_FontCache.find(path) != m_FontCache.end())
				return m_FontCache[path];

			CCRef<VSFont> font = CreateRef<VSFont>(path);
			m_FontCache[path] = font;
			return font;
		}

		std::string FilePath;
	private:
		VSMSDFData* m_Data;
		CCRef<VSTexture2D> m_AtlasTexture;
		static inline std::unordered_map<std::string, CCRef<VSFont>> m_FontCache;
	};
}
