#include "ccpch.h"
#include "BrowserPanel.h"

#include <Cataclysm.h>
#include "Cataclysm/Project/Project.h"

#include <imgui/imgui.h>
#include <Windows.h>
#include <shellapi.h>
#include <filesystem>

namespace Cataclysm
{
	static double lastClickTime = 0.0;
	static const double doubleClickThreshold = 0.3;

	static std::unordered_map <std::string, Ref<Texture2D>> s_ThumbnailCache;

	BrowserPanel::BrowserPanel()
		: m_BaseDirectory(Project::GetAssetDirectory()), m_CurrentDirectory(m_BaseDirectory)
	{
		m_DirectoryIcon = Cataclysm::Texture2D::Create("Resources/Icons/Browser/DirectoryIcon.png");
		m_FileIcon = Cataclysm::Texture2D::Create("Resources/Icons/Browser/FileIcon.png");
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

	static bool IsUnwantedFileType(const std::filesystem::path& path)
	{
		std::string ext = path.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		return (ext == ".bat" || ext == ".lua" || ext == ".csproj" || ext == ".sln");
	}

	static bool IsUnwantedDirectory(const std::string& path)
	{
		return path == "Binaries" ||
			   path == "Intermediates" ||
			   path == ".vs";
	}

	static bool EndsWithFolder(const std::string& path, const std::string& folder) {
		std::string normalizedPath = path;
		std::string normalizedFolder = folder;

		// Remove trailing slashes from path
			while (!normalizedPath.empty() && (normalizedPath.back() == '/' || normalizedPath.back() == '\\')) {
				normalizedPath.pop_back();
			}

		// Prepend slash to folder to ensure it's treated as a full segment
		if (!normalizedFolder.empty() && normalizedFolder.front() != '/' && normalizedFolder.front() != '\\') {
			normalizedFolder = "/" + normalizedFolder;
		}

		// Check if path ends with "/folder" or "\\folder"
		return normalizedPath.size() >= normalizedFolder.size() &&
			(normalizedPath.compare(normalizedPath.size() - normalizedFolder.size(), normalizedFolder.size(), normalizedFolder) == 0 ||
				normalizedPath.compare(normalizedPath.size() - normalizedFolder.size(), normalizedFolder.size(), "\\" + folder) == 0);
	}

	void BrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Project");

		if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 0.0f;
		static float thumbnailSize = 100.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			Cataclysm::Ref<Cataclysm::Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

			float renderedThumbnailSize = thumbnailSize;

			if (IsUnwantedFileType(path) || IsUnwantedDirectory(filenameString))
			{
				// ImGui::NextColumn();

				ImGui::PopID();
				ImGui::PopStyleColor();
				continue;
			}
			else if (IsTextureFile(path))
			{
				std::string fullPath = path.string();
				if (s_ThumbnailCache.find(fullPath) == s_ThumbnailCache.end())
				{
					// Load the texture
					Ref<Texture2D> texture = Texture2D::Create(fullPath);
					if (texture && texture->IsLoaded())
						s_ThumbnailCache[fullPath] = texture;
					else
						s_ThumbnailCache[fullPath] = m_FileIcon; // fallback
				}

				icon = s_ThumbnailCache[fullPath];
				renderedThumbnailSize = thumbnailSize * 0.8f;
			}
			else if (IsCCFile(path))
			{
				std::string fullPath = "..\\Resources\\CataclysmLogo.png";
				if (s_ThumbnailCache.find(fullPath) == s_ThumbnailCache.end())
				{
					// Load the texture
					Ref<Texture2D> texture = Texture2D::Create(fullPath);
					if (texture && texture->IsLoaded())
						s_ThumbnailCache[fullPath] = texture;
					else
						s_ThumbnailCache[fullPath] = m_FileIcon; // fallback
				}

				icon = s_ThumbnailCache[fullPath];
				renderedThumbnailSize = thumbnailSize * 0.8f;
			}
			else if (IsCsharpFile(path))
			{
				std::string fullPath = "..\\Resources\\CsharpLogo.png";
				if (s_ThumbnailCache.find(fullPath) == s_ThumbnailCache.end())
				{
					// Load the texture
					Ref<Texture2D> texture = Texture2D::Create(fullPath);
					if (texture && texture->IsLoaded())
						s_ThumbnailCache[fullPath] = texture;
					else
						s_ThumbnailCache[fullPath] = m_FileIcon; // fallback
				}

				icon = s_ThumbnailCache[fullPath];
				renderedThumbnailSize = thumbnailSize * 0.8f;
			}
			else if (IsFontFile(path))
			{
				std::string fullPath = "..\\Resources\\CsharpLogo.png";
				std::string fontPathStr = path.string();
				Cataclysm::Ref<Font> font = Font::GetFont(fontPathStr);

				if (font && font->GetAtlasTexture())
				{
					Cataclysm::Ref<Texture2D> atlasTexture = font->GetAtlasTexture();

					renderedThumbnailSize = thumbnailSize * 0.8f;
					icon = atlasTexture;
				}
			}
			
			
			if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { renderedThumbnailSize, renderedThumbnailSize }, { 0, 1 }, { 1, 0 }))
			{
				double currentTime = ImGui::GetTime();

				if (currentTime - lastClickTime < doubleClickThreshold)
				{
					if (std::filesystem::is_regular_file(path))
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

				lastClickTime = currentTime;
			}

			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(path);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::Text(filenameString.c_str());
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();

		}

		ImGui::Columns(1);

		// ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		// ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar
		ImGui::End();
	}
}
