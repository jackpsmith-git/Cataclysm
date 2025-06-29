#pragma once

#include <filesystem>
#include <optional>

#include "Cataclysm.h"
#include "Cataclysm/Vesuvius/VSTexture.h"

namespace Cataclysm
{
	class CPBrowserPanel
	{
	public:
		CPBrowserPanel();

		void OnImGuiRender();
		void DirectoryTree(const std::filesystem::path& directory, std::filesystem::path& outSelected);
	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;
		std::optional<std::filesystem::path> m_SelectedEntry;
		bool m_TileView = true;
		bool m_SmallTiles = false;
	};
}
