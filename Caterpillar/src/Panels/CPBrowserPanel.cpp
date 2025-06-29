#include "ccpch.h"
#include "Panels/CPBrowserPanel.h"

#include <Cataclysm.h>
#include "Cataclysm/Project/CCProject.h"
#include "Cataclysm/Audio/CCAudioEngine.h"
#include "Cataclysm/Scripting/CCScriptEngine.h"
#include "Cataclysm/Asset/CCAssetSystem.h"
#include "Cataclysm/Asset/CCAssetMetaSerializer.h"
#include "CPUtils.h"
#include "ImGui\CPImGui.h"
#include "CPStyles.h"

#include <imgui/imgui.h>
#include <Windows.h>
#include <shellapi.h>
#include <filesystem>

namespace Cataclysm
{
	static double lastClickTime = 0.0;
	static const double doubleClickThreshold = 0.3;

	static std::unordered_map <std::string, CCRef<Vesuvius::VSTexture2D>> s_ThumbnailCache;
	static std::unordered_map <std::string, CCRef<Vesuvius::VSTexture2D>> s_AudioPreviewCache;

	static bool openFolderDialog = false;
	static char folderNameBuffer[128] = "";
	static bool openScriptDialog = false;
	static char scriptNameBuffer[128] = "";

	CPBrowserPanel::CPBrowserPanel() : m_BaseDirectory(CCProject::GetAssetDirectory()), m_CurrentDirectory(m_BaseDirectory)
	{
	}

	void CPBrowserPanel::OnImGuiRender()
	{
		CPImGui::BeginPanel("Project");
		CPImGui::BeginPanelMenu();
		{
			if (CPImGui::ImageButton(CPResources::ICON_PLUS, CPStyles::GetPanelMenuHeight()))
			{
				ImGui::OpenPopup("NewItem");
			}

			if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
			{
				ImGui::SameLine();

				if (CPImGui::ImageButton(CPResources::ICON_BACK, CPStyles::GetPanelMenuHeight()))
				{
					m_CurrentDirectory = m_CurrentDirectory.parent_path();
				}
			}

			ImGui::SameLine();
			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - (3.0f * CPStyles::GetPanelMenuHeight()));

			if (m_TileView && m_SmallTiles)
			{
				if (CPImGui::ImageButton(CPResources::ICON_LIST_FADED, CPStyles::GetPanelMenuHeight()))
				{
					m_TileView = false;
				}
				ImGui::SameLine();
				CPImGui::ImageButton(CPResources::ICON_TILES_SMALL, CPStyles::GetPanelMenuHeight());
				ImGui::SameLine();
				if (CPImGui::ImageButton(CPResources::ICON_TILES_FADED, CPStyles::GetPanelMenuHeight()))
				{
					m_TileView = true;
					m_SmallTiles = false;
				}
			}
			else if (m_TileView && !m_SmallTiles)
			{
				if (CPImGui::ImageButton(CPResources::ICON_LIST_FADED, CPStyles::GetPanelMenuHeight()))
				{
					m_TileView = false;
				}
				ImGui::SameLine();
				if (CPImGui::ImageButton(CPResources::ICON_TILES_SMALL_FADED, CPStyles::GetPanelMenuHeight()))
				{
					m_TileView = true;
					m_SmallTiles = true;
				}
				ImGui::SameLine();
				CPImGui::ImageButton(CPResources::ICON_TILES, CPStyles::GetPanelMenuHeight());
			}
			else if (!m_TileView) // LIST VIEW
			{
				CPImGui::ImageButton(CPResources::ICON_LIST, CPStyles::GetPanelMenuHeight());
				ImGui::SameLine();
				if (CPImGui::ImageButton(CPResources::ICON_TILES_SMALL_FADED, CPStyles::GetPanelMenuHeight()))
				{
					m_TileView = true;
					m_SmallTiles = true;
				}
				ImGui::SameLine();
				if (CPImGui::ImageButton(CPResources::ICON_TILES_FADED, CPStyles::GetPanelMenuHeight()))
				{
					m_TileView = true;
					m_SmallTiles = false;
				}
			}
		}
		CPImGui::EndPanelMenu();

		ImGui::PopStyleVar();

		if (ImGui::BeginPopup("NewItem"))
		{
			CPImGui::Image(CPResources::ICON_DIRECTORY, CPStyles::GetIconTreeSize());
			ImGui::SameLine();
			if (ImGui::MenuItem("Folder"))
			{
				memset(folderNameBuffer, 0, sizeof(folderNameBuffer));
				openFolderDialog = true;
			}

			CPImGui::Image(CPResources::ICON_CSHARP_LOGO_320, CPStyles::GetIconTreeSize());
			ImGui::SameLine();
			if (ImGui::MenuItem("MonoScript"))
			{
				memset(scriptNameBuffer, 0, sizeof(scriptNameBuffer));
				openScriptDialog = true;
			}

			ImGui::EndPopup();
		}

		if (openFolderDialog)
		{
			ImGui::OpenPopup("New Folder");
			openFolderDialog = false;
		}

		if (openScriptDialog)
		{
			ImGui::OpenPopup("New Script");
			openScriptDialog = false;
		}

		if (ImGui::BeginPopupModal("New Folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			std::string directory = m_CurrentDirectory.string() + "\\";
			std::string folderNameStr(folderNameBuffer);
			const std::string invalidChars = "\\/:?\"<>|*";

			ImGui::Text(directory.c_str());
			ImGui::SameLine();
			ImGui::InputText("##FolderName", folderNameBuffer, IM_ARRAYSIZE(folderNameBuffer));

			if (ImGui::Button("OK", CPStyles::GetButtonSizeDialog()))
			{
				std::filesystem::path newFolderPath = m_CurrentDirectory / folderNameBuffer;

				if (folderNameStr.empty())
				{
					CC_CORE_WARN("Directory must have a name!");
				}
				else if (folderNameStr.find_first_of(invalidChars) != std::string::npos)
				{
					CC_CORE_WARN("Directory name contains invalid characters!");
				}
				else if (std::filesystem::exists(newFolderPath))
				{
					CC_CORE_WARN("Directory '{}' already exists!", newFolderPath.string());
				}
				else
				{
					std::filesystem::create_directory(newFolderPath);
				}

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", CPStyles::GetButtonSizeDialog()))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("New Script", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			std::string directory = m_CurrentDirectory.string() + "\\";
			std::string scriptNameStr(scriptNameBuffer);
			const std::string invalidChars = "\\/:?\"<>|*";

			ImGui::Text(directory.c_str());
			ImGui::SameLine();
			ImGui::InputText("##ScriptName", scriptNameBuffer, IM_ARRAYSIZE(scriptNameBuffer));
			ImGui::SameLine();
			ImGui::Text(".cs");

			if (ImGui::Button("OK", CPStyles::GetButtonSizeDialog()))
			{
				CPFileSystemUtils::CreateMonoScript(scriptNameStr, m_BaseDirectory, m_CurrentDirectory);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", CPStyles::GetButtonSizeDialog()))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

		CPImGui::BeginPanelContent();

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		float leftPanelWidth = panelSize.x * 0.3f;

		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);
		ImGui::BeginChild("##TreeView", ImVec2(leftPanelWidth, panelSize.y - CPStyles::GetStatusBarHeight()), true);
		{
			DirectoryTree(m_BaseDirectory, m_CurrentDirectory);
		}
		ImGui::PopStyleVar();

		ImGui::EndChild();

		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);

		ImVec2 originalCursorPos = ImGui::GetCursorPos();
		ImVec2 originalCursorScreenPos = ImGui::GetCursorScreenPos();
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		float lineX = originalCursorScreenPos.x + 4;
		drawList->AddLine(
			ImVec2(lineX, windowPos.y),                                
			ImVec2(lineX, windowPos.y + windowSize.y - CPStyles::GetStatusBarHeight() - 5.0f),                 
			IM_COL32(100, 100, 100, 255),                              
			1.0f                                                       
		);
		ImGui::SetCursorPos(ImVec2(originalCursorPos.x + 15, originalCursorPos.y));

		ImGui::BeginChild("##TileView", ImVec2(0, panelSize.y - CPStyles::GetStatusBarHeight()), true);
		{
			if (m_TileView)
			{
				std::string dir = m_CurrentDirectory.string();

				size_t pos = m_CurrentDirectory.string().find("Assets");
				if (pos != std::string::npos)
					dir = m_CurrentDirectory.string().substr(pos);

				std::string dirText = dir + "\\";
				ImGui::Text(dirText.c_str());
				ImGui::Spacing();
				ImGui::Separator();

				static float padding = 0.0f;
				static float thumbnailSize = CPStyles::GetThumbnailSizeLarge();
				if (m_SmallTiles)
					thumbnailSize = CPStyles::GetThumbnailSizeSmall();
				else
					thumbnailSize = CPStyles::GetThumbnailSizeLarge();
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
					Cataclysm::CCRef<Vesuvius::VSTexture2D> icon = directoryEntry.is_directory() ? CPResources::ICON_DIRECTORY : CPResources::ICON_FILE;
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

					float renderedThumbnailSize = thumbnailSize;
					if (CPFileSystemUtils::IsUnwantedFileType(path) || CPFileSystemUtils::IsUnwantedDirectory(filenameString))
					{
						ImGui::PopID();
						ImGui::PopStyleColor();
						continue;
					}

					icon = CPFileSystemUtils::GetFileThumbnail(directoryEntry, s_ThumbnailCache);
					if (icon != CPResources::ICON_DIRECTORY && icon != CPResources::ICON_FILE)
						renderedThumbnailSize = thumbnailSize * 0.8f;

					if (CPImGui::ImageButton(icon, renderedThumbnailSize))
					{
						double currentTime = ImGui::GetTime();
						if (currentTime - lastClickTime < doubleClickThreshold)
						{
							CPFileSystemUtils::OpenFile(path, m_BaseDirectory);
						}

						lastClickTime = currentTime;
					}

					if (ImGui::IsItemClicked())
					{
						m_SelectedEntry = path;
					}

					if (ImGui::BeginPopupContextItem("DirectoryEntry"))
					{
						CPImGui::Image(CPResources::ICON_DELETE, CPStyles::GetIconTreeSize());
						ImGui::SameLine();

						if (ImGui::MenuItem("Delete"))
						{
							try
							{
								if (m_CurrentDirectory == path)
								{
									m_CurrentDirectory = path.parent_path();
									std::filesystem::path nonConstPath = path;
									nonConstPath.clear();
								}

								if (std::filesystem::is_directory(path))
									std::filesystem::remove_all(path);
								else
									std::filesystem::remove(path);

								if (CCAssetSystem::GetAssetType(path) != CCAssetType::Unknown)
								{
									std::filesystem::path metaPath = path;
									metaPath += ".meta";

									if (std::filesystem::exists(metaPath))
										std::filesystem::remove(metaPath);
								}
							}
							catch (const std::exception& e)
							{
								CC_CORE_WARN("[CPBrowserPanel::DirectoryTree] Failed to delete directory entry!");
							}
						}

						ImGui::EndPopup();
					}

					if (ImGui::BeginDragDropSource())
					{
						std::filesystem::path relativePath(path);
						const wchar_t* itemPath = relativePath.c_str();
						ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
						CPImGui::Image(icon, 20);
						ImGui::SameLine();
						ImGui::Text(filenameString.c_str());
						ImGui::EndDragDropSource();
					}

					if (directoryEntry.is_directory())
					{
						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
							{
								const wchar_t* srcPath = (const wchar_t*)payload->Data;
								std::filesystem::path source(srcPath);
								std::filesystem::path destination = path / source.filename();

								if (std::filesystem::exists(destination))
								{
									CC_CORE_WARN("[CPBrowserPanel::OnImGuiRender] Failed to move directory entry. Entry already exists in destination directory!");
								}

								try
								{
									std::filesystem::rename(source, destination);

									std::filesystem::path metaPath = source;
									metaPath += ".meta";

									if (std::filesystem::exists(metaPath))
									{
										// Update metadata file path
										CCAssetMetadata metadata = CCAssetMetaSerializer::DeserializeMetaFile(metaPath);
										CCUUID id = metadata.ID;
										CCAssetType type = metadata.Type;
										CCAssetMetaSerializer::SerializeMetaFile(destination, id, type);

										std::filesystem::remove(metaPath); // Remove old meta file
									}
								}
								catch (const std::exception& e)
								{
									CC_CORE_WARN("[CPBrowserPanel::OnImGuiRender] Failed to move file!");
								}

							}

							ImGui::EndDragDropTarget();
						}
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
			}
			else
			{
				std::string dir = m_CurrentDirectory.string();

				size_t pos = m_CurrentDirectory.string().find("Assets");
				if (pos != std::string::npos)
					dir = m_CurrentDirectory.string().substr(pos);

				std::string dirText = dir + "\\";
				ImGui::Text(dirText.c_str());
				ImGui::Spacing();
				ImGui::Separator();

				for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
				{
					const auto& path = directoryEntry.path();
					std::string filenameString = path.filename().string();

					ImGui::PushID(filenameString.c_str());
					Cataclysm::CCRef<Vesuvius::VSTexture2D> icon = directoryEntry.is_directory() ? CPResources::ICON_DIRECTORY : CPResources::ICON_FILE;
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

					if (CPFileSystemUtils::IsUnwantedFileType(path) || CPFileSystemUtils::IsUnwantedDirectory(filenameString))
					{
						ImGui::PopID();
						ImGui::PopStyleColor();
						continue;
					}

					icon = CPFileSystemUtils::GetFileThumbnail(directoryEntry, s_ThumbnailCache);

					CPImGui::Image(icon, CPStyles::GetIconTreeSize());
					ImGui::SameLine();

					if (ImGui::TreeNodeEx(filenameString.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth))
					{
						if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							CPFileSystemUtils::OpenFile(path, m_BaseDirectory);
						}

						ImGui::TreePop();
					};

					if (ImGui::IsItemClicked())
					{
						m_SelectedEntry = path;
					}

					if (ImGui::BeginPopupContextItem("DirectoryEntry"))
					{
						CPImGui::Image(CPResources::ICON_DELETE, CPStyles::GetIconTreeSize());
						ImGui::SameLine();

						if (ImGui::MenuItem("Delete"))
						{
							try
							{
								if (std::filesystem::is_directory(path))
									std::filesystem::remove_all(path);
								else
									std::filesystem::remove(path);

								if (CCAssetSystem::GetAssetType(path) != CCAssetType::Unknown)
								{
									std::filesystem::path metaPath = path;
									metaPath += ".meta";

									if (std::filesystem::exists(metaPath))
										std::filesystem::remove(metaPath);
								}

								if (m_CurrentDirectory == path)
								{
									m_CurrentDirectory.clear();
									m_CurrentDirectory = path.parent_path();
								}
							}
							catch (const std::exception& e)
							{
								CC_CORE_WARN("[CPBrowserPanel::DirectoryTree] Failed to delete directory entry!");
							}
						}

						ImGui::EndPopup();
					}

					if (ImGui::BeginDragDropSource())
					{
						std::filesystem::path relativePath(path);
						const wchar_t* itemPath = relativePath.c_str();
						ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
						CPImGui::Image(icon, 20);
						ImGui::SameLine();
						ImGui::Text(filenameString.c_str());
						ImGui::EndDragDropSource();
					}

					if (directoryEntry.is_directory())
					{
						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
							{
								const wchar_t* srcPath = (const wchar_t*)payload->Data;
								std::filesystem::path source(srcPath);
								std::filesystem::path destination = path / source.filename();

								if (std::filesystem::exists(destination))
								{
									CC_CORE_WARN("[CPBrowserPanel::OnImGuiRender] Failed to move directory entry. Entry already exists in destination directory!");
								}

								try
								{
									std::filesystem::rename(source, destination);

									std::filesystem::path metaPath = source;
									metaPath += ".meta";

									if (std::filesystem::exists(metaPath))
									{
										// Update metadata file path
										CCAssetMetadata metadata = CCAssetMetaSerializer::DeserializeMetaFile(metaPath);
										CCUUID id = metadata.ID;
										CCAssetType type = metadata.Type;
										CCAssetMetaSerializer::SerializeMetaFile(destination, id, type);

										std::filesystem::remove(metaPath); // Remove old meta file
									}
								}
								catch (const std::exception& e)
								{
									CC_CORE_WARN("[CPBrowserPanel::OnImGuiRender] Failed to move file!");
								}

							}

							ImGui::EndDragDropTarget();
						}
					}

					ImGui::PopStyleColor();

					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						if (directoryEntry.is_directory())
							m_CurrentDirectory /= path.filename();
					}

					ImGui::PopID();
					ImGui::Separator();
				}
			}

			ImGui::Columns(1);
		}
		ImGui::PopStyleVar();
		ImGui::EndChild();

		CPImGui::EndPanelContent();
		CPImGui::EndPanel();

		//CPImGui::BeginPanel("Asset");
		//CPImGui::BeginPanelContent();

		//if (m_SelectedEntry.has_value())
		//{
		//	ImGui::TextWrapped(m_SelectedEntry.value().string().c_str());

		//	if (CPFileSystemUtils::IsAudioFile(m_SelectedEntry.value()) ||
		//		CPFileSystemUtils::IsFontFile(m_SelectedEntry.value()) ||
		//		CPFileSystemUtils::IsTextureFile(m_SelectedEntry.value()))
		//	{
		//		CCUUID ccuuid = CCAssetSystem::GetUUIDFromPath(m_SelectedEntry.value().string());
		//		uint64_t id = (uint64_t)ccuuid;
		//		ImGui::TextWrapped(std::to_string(id).c_str());
		//	}
		//}
		//
		//CPImGui::EndPanelContent();
		//CPImGui::EndPanel();
	}

	void CPBrowserPanel::DirectoryTree(const std::filesystem::path& directory, std::filesystem::path& outSelected)
	{
		std::string rootName = directory.filename().string();
		if (rootName.empty())
			rootName = directory.string();


		ImGuiID nodeID = ImGui::GetID(rootName.c_str());
		ImGuiStorage* storage = ImGui::GetStateStorage();
		bool wasOpen = storage->GetBool(nodeID, false);

		CPImGui::Image(wasOpen ? CPResources::ICON_DIRECTORY_OPEN : CPResources::ICON_DIRECTORY, CPStyles::GetIconTreeSize());
		ImGui::SameLine();

		bool open = ImGui::TreeNodeEx(rootName.c_str(),
			(outSelected == directory ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);

		storage->SetBool(nodeID, open);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* srcPath = (const wchar_t*)payload->Data;
				std::filesystem::path source(srcPath);
				std::filesystem::path destination = directory / source.filename();

				if (std::filesystem::exists(destination))
				{
					CC_CORE_WARN("[CPBrowserPanel::OnImGuiRender] Failed to move directory entry. Entry already exists in destination directory!");
				}

				try
				{
					std::filesystem::rename(source, destination);

					std::filesystem::path metaPath = source;
					metaPath += ".meta";

					if (std::filesystem::exists(metaPath))
					{
						// Update metadata file path
						CCAssetMetadata metadata = CCAssetMetaSerializer::DeserializeMetaFile(metaPath);
						CCUUID id = metadata.ID;
						CCAssetType type = metadata.Type;
						CCAssetMetaSerializer::SerializeMetaFile(destination, id, type);

						std::filesystem::remove(metaPath); // Remove old meta file
					}
				}
				catch (const std::exception& e)
				{
					CC_CORE_WARN("[CPBrowserPanel::OnImGuiRender] Failed to move file!");
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemClicked())
		{
			outSelected = directory;
			m_SelectedEntry = directory;
		}

		if (open)
		{
			for (const auto& entry : std::filesystem::directory_iterator(directory))
			{
				const auto& path = entry.path();
				std::string filenameString = path.filename().string();

				if (!entry.is_directory() || CPFileSystemUtils::IsUnwantedFileType(path) || CPFileSystemUtils::IsUnwantedDirectory(filenameString))
					continue;

				DirectoryTree(path, outSelected);
			}

			ImGui::TreePop();
		}
	}
}
