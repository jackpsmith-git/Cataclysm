#pragma once

#include <filesystem>

#include "Cataclysm.h"
#include "Cataclysm/Renderer/Texture.h"

namespace Cataclysm
{
	class BrowserPanel
	{
	public:
		BrowserPanel();

		void OnImGuiRender();
	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;

		Cataclysm::Ref<Cataclysm::Texture2D> m_DirectoryIcon;
		Cataclysm::Ref<Cataclysm::Texture2D> m_FileIcon;
	};
}
