#include "Panels/CPSceneHierarchyPanel.h"
#include "Cataclysm/ECS/CCComponents.h"

#include "Cataclysm/Audio/CCAudioEngine.h"
#include "Cataclysm/Scripting/CCScriptEngine.h"
#include "Cataclysm/ImGui/CCImGuiScopedStyleColor.h"
#include "Cataclysm/Asset/CCAssetSystem.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"

#include "CPStyles.h"
#include "ImGui\CPImGui.h"
#include "CPUtils.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <glm/gtc/type_ptr.hpp>

#ifdef _MSVC_LANG
	#define _CRT_SECURE_NO_WARNINGS
#endif

namespace Cataclysm
{
	CPSceneHierarchyPanel::CPSceneHierarchyPanel() {}
	CPSceneHierarchyPanel::CPSceneHierarchyPanel(const CCRef<CCScene>& context) { SetContext(context); }

	void CPSceneHierarchyPanel::SetContext(const CCRef<CCScene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void CPSceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		auto view = m_Context->m_Registry.view<IDComponent, TagComponent>();

		for (auto e : view)
		{
			Entity entity = { e, m_Context.get() };
			CCUUID id = entity.GetUUID();

			if (CCSceneEntityManager::GetParent(m_Context.get(), id) == 0)
				CPImGui::EntityNode(entity, m_Context, m_SelectionContext);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		ImVec2 dropAreaSize = ImGui::GetContentRegionAvail();

		ImGui::InvisibleButton("HierarchyDropTarget", dropAreaSize);

		if (ImGui::BeginPopupContextItem())
		{
			CPImGui::Image(CPResources::ICON_ENTITY, CPStyles::GetIconTreeSize(), CPStyles::ICON_COLOR_HIERARCHY);
			ImGui::SameLine();
			if (ImGui::BeginMenu("New Entity"))
			{
				CPImGui::Image(CPResources::ICON_ENTITY, CPStyles::GetIconTreeSize(), CPStyles::ICON_COLOR_HIERARCHY);
				ImGui::SameLine();
				if (ImGui::MenuItem("Empty Entity"))
					CCSceneEntityManager::CreateEntity(m_Context.get(), "Empty Entity");

				CPImGui::Image(CPResources::ICON_AUDIO_SOURCE, CPStyles::GetIconTreeSize(), CPStyles::ICON_COLOR_HIERARCHY);
				ImGui::SameLine();
				if (ImGui::MenuItem("AudioSource"))
				{
					Entity entity = CCSceneEntityManager::CreateEntity(m_Context.get(), "AudioSource");
					entity.AddComponent<AudioSource>();
				}

				CPImGui::Image(CPResources::ICON_CAMERA, CPStyles::GetIconTreeSize(), CPStyles::ICON_COLOR_HIERARCHY);
				ImGui::SameLine();
				if (ImGui::MenuItem("Camera"))
				{
					Entity entity = CCSceneEntityManager::CreateEntity(m_Context.get(), "Camera");
					entity.AddComponent<Camera>();
				}

				CPImGui::Image(CPResources::ICON_RIGIDBODY_2D, CPStyles::GetIconTreeSize(), CPStyles::ICON_COLOR_HIERARCHY);
				ImGui::SameLine();
				if (ImGui::MenuItem("Mass"))
				{
					Entity entity = CCSceneEntityManager::CreateEntity(m_Context.get(), "Mass");
					entity.AddComponent<Rigidbody2D>();
					entity.AddComponent<SpriteRenderer>();
					entity.AddComponent<BoxCollider2D>();
				}

				CPImGui::Image(CPResources::ICON_SPRITE_RENDERER, CPStyles::GetIconTreeSize(), CPStyles::ICON_COLOR_HIERARCHY);
				ImGui::SameLine();
				if (ImGui::MenuItem("Sprite"))
				{
					Entity entity = CCSceneEntityManager::CreateEntity(m_Context.get(), "Sprite");
					entity.AddComponent<SpriteRenderer>();
				}

				CPImGui::Image(CPResources::ICON_TEXT, CPStyles::GetIconTreeSize(), CPStyles::ICON_COLOR_HIERARCHY);
				ImGui::SameLine();
				if (ImGui::MenuItem("Text"))
				{
					Entity entity = CCSceneEntityManager::CreateEntity(m_Context.get(), "Text");
					entity.AddComponent<Text>();
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP"))
			{
				CCUUID droppedID = *(const CCUUID*)payload->Data;
				CCSceneEntityManager::RemoveParent(m_Context.get(), droppedID);
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::TextWrapped(" ");

		ImGui::End();
	}

	void CPSceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	template<typename T, typename UIFunction>
	void CPSceneHierarchyPanel::DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 1 });
				float lineHeight = CPStyles::GetIconSize().x;
				ImGui::Separator();
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
				std::string nodeID = std::to_string(entity.GetUUID()) + std::to_string(typeid(T).hash_code());
				bool open = CPImGui::TreeNode(nodeID, CPResources::GetComponentIcon<T>(), treeNodeFlags, name.c_str());
				ImGui::PopStyleVar();
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			
			ImGui::PushFont(boldFont);
				std::string popupName = "ComponentSettings##" + std::to_string(entity.GetUUID()) + typeid(T).name();
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
				if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup(popupName.c_str());
				}
				ImGui::PopStyleVar();
			ImGui::PopFont();

			bool removeComponent = false;
			bool resetComponent = false;

			if (ImGui::BeginPopup(popupName.c_str()))
			{
				if ((!std::is_same<T, MonoScript>::value) &&
					(!std::is_same<T, IDComponent>::value) &&
					(!std::is_same<T, NativeScript>::value))
				{
					CPImGui::Image(CPResources::ICON_RESET, CPStyles::GetIconTreeSize());
					ImGui::SameLine();

					if (ImGui::MenuItem("Reset Component"))
						resetComponent = true;
				}

				if (!std::is_same<T, Transform>::value)
				{
					CPImGui::Image(CPResources::ICON_DELETE, CPStyles::GetIconTreeSize());
					ImGui::SameLine();

					if (ImGui::MenuItem("Remove Component"))
						removeComponent = true;

				}

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (resetComponent)
				component.Reset();

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	template<typename T>
	void CPSceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) 
	{
		if (!m_SelectionContext.HasComponent<T>())
		{
			CPImGui::Image(CPResources::GetComponentIcon<T>(), CPStyles::GetIconTreeSize());
			ImGui::SameLine();
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void CPSceneHierarchyPanel::DrawComponents(Entity entity)
	{
		CPImGui::Image(CPResources::ICON_ENTITY, CPStyles::GetIconSize());
		ImGui::SameLine();

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		auto& id = entity.GetComponent<IDComponent>();

		ImVec2 startPos = ImGui::GetCursorScreenPos(); // Top-left before drawing
		float spacing = 0.0f; // Some padding around the composite
		float rounding = CPStyles::GetElementRounding();
		float height = CPStyles::GetIconSize().x; // Total height of control
		ImVec2 fullSize = ImVec2(ImGui::GetContentRegionAvail().x, height);

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(ImVec2(startPos.x - 4.0f, startPos.y - spacing),
			ImVec2(startPos.x + fullSize.x + 4.0f, startPos.y + fullSize.y + spacing),
			ImGui::GetColorU32(ImGuiCol_FrameBg),
			rounding
		);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 0 });
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
		CPImGui::TagCheckbox(&id.Enabled);
		ImGui::PopStyleVar();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::SameLine();

		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - CPStyles::GetIconSize().x);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
		ImGui::PushFont(boldFont);
		if (ImGui::Button("+", CPStyles::GetIconSize()))
			ImGui::OpenPopup("AddComponent");
		ImGui::PopFont();
		ImGui::PopStyleVar(3);

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<AudioSource>("AudioSource");
			DisplayAddComponentEntry<BoxCollider2D>("BoxCollider2D");
			DisplayAddComponentEntry<Camera>("Camera");
			DisplayAddComponentEntry<CircleCollider2D>("CircleCollider2D");
			DisplayAddComponentEntry<CircleRenderer>("CircleRenderer");
			DisplayAddComponentEntry<MonoScript>("MonoScript");
			DisplayAddComponentEntry<Rigidbody2D>("Rigidbody2D");
			DisplayAddComponentEntry<SpriteRenderer>("SpriteRenderer");
			DisplayAddComponentEntry<Text>("Text");

			ImGui::EndPopup();
		}

		ImGui::Spacing();
		ImGui::Spacing();

		DrawComponent<Transform>("Transform", entity, [=](auto& component)
		{
			CPImGui::DragVec3("Translation", std::to_string(id.ID) + "_Transform_Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			CPImGui::DragVec3("Rotation", std::to_string(id.ID) + "_Transform_Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			CPImGui::DragVec3("Scale", std::to_string(id.ID) + "_Transform_Scale", component.Scale, 1.0f);
		});

		DrawComponent<Camera>("Camera", entity, [](auto& component)
		{
				auto& camera = component.SceneCamera;

				CPImGui::FramedCheckbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				if (CPImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((CCSceneCamera::CCProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}		
				ImGui::Columns(1);

				if (camera.GetProjectionType() == CCSceneCamera::CCProjectionType::Perspective)
				{
					float fov = glm::degrees(camera.GetPerspectiveVerticalFOV());

					if (CPImGui::FramedDragFloat("FOV", &fov))
						camera.SetPerspectiveVerticalFOV(glm::radians(fov));

					float perspNearClip = camera.GetPerspectiveNearClip();
					if (CPImGui::FramedDragFloat("Near Clip", &perspNearClip))
						camera.SetPerspectiveNearClip(perspNearClip);

					float perspFarClip = camera.GetPerspectiveFarClip();
					if (CPImGui::FramedDragFloat("Far Clip", &perspFarClip))
						camera.SetPerspectiveFarClip(perspFarClip);
				}

				if (camera.GetProjectionType() == CCSceneCamera::CCProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (CPImGui::FramedDragFloat("FOV", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNearClip = camera.GetOrthographicNearClip();
					if (CPImGui::FramedDragFloat("Near Clip", &orthoNearClip))
						camera.SetOrthographicNearClip(orthoNearClip);

					float orthoFarClip = camera.GetOrthographicFarClip();
					if (CPImGui::FramedDragFloat("Far Clip", &orthoFarClip))
						camera.SetOrthographicFarClip(orthoFarClip);

					CPImGui::FramedCheckbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});

		DrawComponent<MonoScript>("MonoScript", entity, [entity, scene = m_Context](auto& component) mutable
			{
				bool scriptClassExists = CCScriptEngine::EntityClassExists(component.ClassName);

				static char buffer[64];
				strcpy_s(buffer, sizeof(buffer), component.ClassName.c_str());

				UI::CCImGuiScopedStyleColor textColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f), !scriptClassExists);

				const char* label = scriptClassExists ? component.ClassName.c_str() : "NULL";
				CPImGui::FramedButtonWithIcon("MonoScript", CPResources::ICON_CSHARP_LOGO_320, label);

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = static_cast<const wchar_t*>(payload->Data);

						// Convert wide string path to std::string
						std::filesystem::path scriptPath = path;

						if (scriptPath.extension() == ".cs")
						{
							std::string filename = scriptPath.filename().string(); // e.g., MyScript.cs

							std::string className = filename.substr(0, filename.find_last_of('.'));
							component.ClassName = className;

							std::string fullClassName = CPFileSystemUtils::ParseCSharpClassFullName(scriptPath);
							if (!fullClassName.empty())
								component.ClassName = fullClassName;
						}
						else
						{
							CC_CORE_WARN("[CPSceneHierarchyPanel::DrawComponent<MonoScript>] MonoScript must be a .cs file!");
						}
					}

					ImGui::EndDragDropTarget();
				}
				ImGui::Columns(1);

				// Fields
				bool sceneRunning = scene->IsRunning();
				if (sceneRunning)
				{
					//CCRef<CCScriptInstance> scriptInstance = CCScriptEngine::GetEntityScriptInstance(entity.GetUUID());
					if (scriptClassExists)
					{
						CCRef<CCScriptClass> entityClass = CCScriptEngine::GetEntityClass(component.ClassName);
						const auto& fields = entityClass->GetFields();

						auto& entityFields = CCScriptEngine::GetScriptFieldMap(entity);

						for (const auto& [name, field] : fields)
						{
							if (entityFields.find(name) != entityFields.end())
							{
								CCScriptFieldInstance& scriptField = entityFields.at(name);

								switch (field.Type)
								{
								case CCScriptFieldType::Float:
								{
									float data = scriptField.GetValue<float>();
									if (CPImGui::FramedDragFloat(name.c_str(), &data))
										scriptField.SetValue(data);
									break;
								}
								case CCScriptFieldType::Bool:
								{
									bool data = scriptField.GetValue<bool>();
									if (CPImGui::FramedCheckbox(name.c_str(), &data))
										scriptField.SetValue(data);
									break;
								}
								case CCScriptFieldType::Int:
								{
									int data = scriptField.GetValue<int>();
									if (CPImGui::FramedDragInt(name.c_str(), &data))
									{
										scriptField.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::Double:
								{
									double data = scriptField.GetValue<double>();
									if (CPImGui::InputDouble(name.c_str(), &data))
									{
										scriptField.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::Vec2:
								{
									glm::vec2 data = scriptField.GetValue<glm::vec2>();
									const std::string& uniqueID = std::to_string(entity.GetUUID()) + "_MonoScript_" + name;
									if (CPImGui::DragVec2(name, uniqueID, data))
									{
										scriptField.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::Vec3:
								{
									glm::vec3 data = scriptField.GetValue<glm::vec3>();
									const std::string& uniqueID = std::to_string(entity.GetUUID()) + "_MonoScript_" + name;
									if (CPImGui::DragVec3(name, uniqueID, data))
									{
										scriptField.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::Vec4:
								{
									glm::vec4 data = scriptField.GetValue<glm::vec4>();
									const std::string& uniqueID = std::to_string(entity.GetUUID()) + "_MonoScript_" + name;
									if (CPImGui::DragVec4(name, uniqueID, data))
									{
										scriptField.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::String:
								{
									std::string data = scriptField.GetValue<std::string>();
									if (CPImGui::InputTextMultiline(scriptField.Field.Name.c_str(), &data))
									{
										scriptField.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::Entity:
								{
									CCUUID entityID = scriptField.GetValue<CCUUID>();
									Entity storedEntity = CCSceneEntityManager::GetEntityByUUID(scene.get(), entityID);

									std::string buttonLabel = "Entity (Null)";
									if (storedEntity && storedEntity.HasComponent<TagComponent>())
									{
										buttonLabel = storedEntity.GetComponent<TagComponent>().Tag;
									}

									CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_ENTITY, buttonLabel.c_str());
									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP"))
										{
											CCUUID entityID = *(const CCUUID*)payload->Data;
											scriptField.SetValue(entityID);
										}
										ImGui::EndDragDropTarget();
									}
									ImGui::Columns(1);

									break;
								}
								case CCScriptFieldType::Texture2D:
								{
									CCUUID assetID = scriptField.GetValue<CCUUID>();
									std::string buttonLabel = "Texture2D (NULL)";
									CCRef<Vesuvius::VSTexture2D> icon = CPResources::ICON_TEXTURE_2D;
									if (assetID != 0)
									{
										CCRef<Vesuvius::VSTexture2D> thumbnail = CCAssetSystem::GetAsset<Vesuvius::VSTexture2D>(assetID);
										icon = thumbnail;
										std::filesystem::path pathstr = thumbnail->GetPath();
										std::filesystem::path path = pathstr;
										std::filesystem::path label = path.filename();
										buttonLabel = label.string();
									}

									CPImGui::FramedButtonWithIcon(name.c_str(), icon, buttonLabel.c_str());

									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
										{
											const wchar_t* path = (const wchar_t*)payload->Data;
											if (CPFileSystemUtils::IsTextureFile(path))
											{
												std::wstring wpath(path);
												std::string pathstr(wpath.begin(), wpath.end());
												CCUUID uuid = Cataclysm::CCAssetSystem::GetUUIDFromPath(pathstr);
												scriptField.SetValue(uuid);
											}
										}
										ImGui::EndDragDropTarget();
									}

									break;
								}
								case CCScriptFieldType::AudioClip:
								{
									CCUUID assetID = scriptField.GetValue<CCUUID>();
									std::string buttonLabel = "AudioClip (NULL)";
									if (assetID != 0)
									{
										std::filesystem::path pathstr = CCAssetSystem::GetAsset<CCAudioClip>(assetID)->GetFilePath();
										std::filesystem::path path = pathstr;
										std::filesystem::path label = path.filename();
										buttonLabel = label.string();
									}

									CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_AUDIO_CLIP, buttonLabel.c_str());

									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
										{
											const wchar_t* path = (const wchar_t*)payload->Data;
											if (CPFileSystemUtils::IsAudioFile(path))
											{
												std::wstring wpath(path);
												std::string pathstr(wpath.begin(), wpath.end());
												CCUUID uuid = Cataclysm::CCAssetSystem::GetUUIDFromPath(pathstr);
												scriptField.SetValue(uuid);
											}
										}
										ImGui::EndDragDropTarget();
									}

									break;
								}
								case CCScriptFieldType::Font:
								{
									CCUUID assetID = scriptField.GetValue<CCUUID>();
									std::string buttonLabel = "Font (NULL)";
									if (assetID != 0)
									{
										auto& asset = CCAssetSystem::GetAsset<Vesuvius::VSFont>(assetID);
										std::string filepath = asset->FilePath;
										// CC_CORE_WARN(filepath);

										std::filesystem::path path = filepath;
										std::filesystem::path label = path.filename();
										buttonLabel = label.string(); // "Font";
									}

									CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_FONT, buttonLabel.c_str());

									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
										{
											const wchar_t* path = (const wchar_t*)payload->Data;
											if (CPFileSystemUtils::IsFontFile(path))
											{
												std::wstring wpath(path);
												std::string pathstr(wpath.begin(), wpath.end());
												CCUUID uuid = Cataclysm::CCAssetSystem::GetUUIDFromPath(pathstr);
												scriptField.SetValue(uuid);
											}
										}
										ImGui::EndDragDropTarget();
									}

									break;
								}
								default:
								{
									break;
								}
								}
							}
							else
							{
								switch (field.Type)
								{
								case CCScriptFieldType::Float:
								{
									float data = 0.0f;
									if (CPImGui::FramedDragFloat(name.c_str(), &data))
									{
										CCScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::Bool:
								{
									bool data = false;
									if (CPImGui::FramedCheckbox(name.c_str(), &data))
									{
										CCScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::Int:
								{
									int data = 0;
									if (CPImGui::FramedDragInt(name.c_str(), &data))
									{
										CCScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::Double:
								{
									double data = 0.0;
									if (CPImGui::InputDouble(name.c_str(), &data))
									{
										CCScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::Vec2:
								{
									glm::vec2 data = glm::vec2(0.0f, 0.0f);
									const std::string& uniqueID = std::to_string(entity.GetUUID()) + "_MonoScript_" + name;
									if (CPImGui::DragVec2(name, uniqueID, data))
									{
										CCScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::Vec3:
								{
									glm::vec3 data = glm::vec3(0.0f, 0.0f, 0.0f);
									const std::string& uniqueID = std::to_string(entity.GetUUID()) + "_MonoScript_" + name;
									if (CPImGui::DragVec3(name, uniqueID, data))
									{
										CCScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::Vec4:
								{
									glm::vec4 data = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
									const std::string& uniqueID = std::to_string(entity.GetUUID()) + "_MonoScript_" + name;
									if (CPImGui::DragVec4(name, uniqueID, data))
									{
										CCScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::String:
								{
									std::string data = std::string();
									if (CPImGui::InputTextMultiline(name.c_str(), &data))
									{
										CCScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
									break;
								}
								case CCScriptFieldType::Entity:
								{
									CCUUID entityUID = entityFields[name].GetValue<CCUUID>();
									Entity storedEntity = CCSceneEntityManager::GetEntityByUUID(scene.get(), entityUID);

									std::string buttonLabel = "Entity (Null)";
									if (storedEntity && storedEntity.HasComponent<TagComponent>())
									{
										buttonLabel = storedEntity.GetComponent<TagComponent>().Tag;
									}

									CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_ENTITY, buttonLabel.c_str());

									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP"))
										{
											CCUUID entityID = *(const CCUUID*)payload->Data;
											CCScriptFieldInstance& fieldInstance = entityFields[name];
											fieldInstance.Field = field;
											fieldInstance.SetValue(entityID);
										}
										ImGui::EndDragDropTarget();
									}
									ImGui::Columns(1);

									break;
								}
								case CCScriptFieldType::Texture2D:
								{
									CCUUID assetID = entityFields[name].GetValue<CCUUID>();
									std::string buttonLabel = "Texture2D (NULL)";
									if (assetID != 0)
									{
										std::string pathstr = CCAssetSystem::GetAsset<Vesuvius::VSTexture2D>(assetID)->GetPath();
										std::filesystem::path path = pathstr;
										std::filesystem::path label = path.filename();
										buttonLabel = label.string();
									}

									CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_TEXTURE_2D, buttonLabel.c_str());

									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
										{
											const wchar_t* path = (const wchar_t*)payload->Data;
											if (CPFileSystemUtils::IsTextureFile(path))
											{
												std::wstring wpath(path);
												std::string pathstr(wpath.begin(), wpath.end());
												CCUUID uuid = Cataclysm::CCAssetSystem::GetUUIDFromPath(pathstr);
												CCScriptFieldInstance& fieldInstance = entityFields[name];
												fieldInstance.Field = field;
												fieldInstance.SetValue(uuid);
											}
										}
										ImGui::EndDragDropTarget();
									}

									break;
								}
								case CCScriptFieldType::AudioClip:
								{
									CCUUID assetID = entityFields[name].GetValue<CCUUID>();
									std::string buttonLabel = "AudioClip (NULL)";
									if (assetID != 0)
										buttonLabel = CCAssetSystem::GetAsset<CCAudioClip>(assetID)->GetFilePath();

									CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_AUDIO_CLIP, buttonLabel.c_str());

									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
										{
											const wchar_t* path = (const wchar_t*)payload->Data;
											if (CPFileSystemUtils::IsAudioFile(path))
											{
												std::wstring wpath(path);
												std::string pathstr(wpath.begin(), wpath.end());
												CCUUID uuid = Cataclysm::CCAssetSystem::GetUUIDFromPath(pathstr);
												CCScriptFieldInstance& fieldInstance = entityFields[name];
												fieldInstance.Field = field;
												fieldInstance.SetValue(uuid);
											}
										}
										ImGui::EndDragDropTarget();
									}

									break;
								}
								case CCScriptFieldType::Font:
								{
									CCUUID assetID = entityFields[name].GetValue<CCUUID>();
									std::string buttonLabel = "Font (NULL)";
									if (assetID != 0)
										buttonLabel = CCAssetSystem::GetAsset<Vesuvius::VSFont>(assetID)->FilePath;

									CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_FONT, buttonLabel.c_str());

									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
										{
											const wchar_t* path = (const wchar_t*)payload->Data;
											if (CPFileSystemUtils::IsFontFile(path))
											{
												std::wstring wpath(path);
												std::string pathstr(wpath.begin(), wpath.end());
												CCUUID uuid = Cataclysm::CCAssetSystem::GetUUIDFromPath(pathstr);
												CCScriptFieldInstance& fieldInstance = entityFields[name];
												fieldInstance.Field = field;
												fieldInstance.SetValue(uuid);
											}
										}
										ImGui::EndDragDropTarget();
									}

									break;
								}
								default:
								{
									break;
								}
								}
							}
						}
					}
				}
				else
				{
					if (scriptClassExists)
					{
						CCRef<CCScriptClass> entityClass = CCScriptEngine::GetEntityClass(component.ClassName);
						const auto& fields = entityClass->GetFields();

						auto& entityFields = CCScriptEngine::GetScriptFieldMap(entity);

						for (const auto& [name, field] : fields)
						{
							if (entityFields.find(name) != entityFields.end())
							{
								CCScriptFieldInstance& scriptField = entityFields.at(name);

								switch (field.Type)
								{
									case CCScriptFieldType::Float:
									{
										float data = scriptField.GetValue<float>();
										if (CPImGui::FramedDragFloat(name.c_str(), &data))
											scriptField.SetValue(data);
										break;
									}
									case CCScriptFieldType::Bool:
									{
										bool data = scriptField.GetValue<bool>();
										if (CPImGui::FramedCheckbox(name.c_str(), &data))
											scriptField.SetValue(data);
										break;
									}
									case CCScriptFieldType::Int:
									{
										int data = scriptField.GetValue<int>();
										if (CPImGui::FramedDragInt(name.c_str(), &data))
										{
											scriptField.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::Double:
									{
										double data = scriptField.GetValue<double>();
										if (CPImGui::InputDouble(name.c_str(), &data))
										{
											scriptField.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::Vec2:
									{
										glm::vec2 data = scriptField.GetValue<glm::vec2>();
										const std::string& uniqueID = std::to_string(entity.GetUUID()) + "_MonoScript_" + name;
										if (CPImGui::DragVec2(name, uniqueID, data))
										{
											scriptField.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::Vec3:
									{
										glm::vec3 data = scriptField.GetValue<glm::vec3>();
										const std::string& uniqueID = std::to_string(entity.GetUUID()) + "_MonoScript_" + name;
										if (CPImGui::DragVec3(name, uniqueID, data))
										{
											scriptField.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::Vec4:
									{
										glm::vec4 data = scriptField.GetValue<glm::vec4>();
										const std::string& uniqueID = std::to_string(entity.GetUUID()) + "_MonoScript_" + name;
										if (CPImGui::DragVec4(name, uniqueID, data))
										{
											scriptField.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::String:
									{
										std::string data = scriptField.GetValue<std::string>();
										if (CPImGui::InputTextMultiline(scriptField.Field.Name.c_str(), &data))
										{
											scriptField.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::Entity:
									{
										CCUUID entityID = scriptField.GetValue<CCUUID>();
										Entity storedEntity = CCSceneEntityManager::GetEntityByUUID(scene.get(), entityID);

										std::string buttonLabel = "Entity (Null)";
										if (storedEntity && storedEntity.HasComponent<TagComponent>())
										{
											buttonLabel = storedEntity.GetComponent<TagComponent>().Tag;
										}

										CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_ENTITY, buttonLabel.c_str());
										if (ImGui::BeginDragDropTarget())
										{
											if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP"))
											{
												CCUUID entityID = *(const CCUUID*)payload->Data;
												scriptField.SetValue(entityID);
											}
											ImGui::EndDragDropTarget();
										}
										ImGui::Columns(1);

										break;
									}
									case CCScriptFieldType::Texture2D:
									{
										CCUUID assetID = scriptField.GetValue<CCUUID>();
										std::string buttonLabel = "Texture2D (NULL)";
										CCRef<Vesuvius::VSTexture2D> icon = CPResources::ICON_TEXTURE_2D;
										if (assetID != 0)
										{
											CCRef<Vesuvius::VSTexture2D> thumbnail = CCAssetSystem::GetAsset<Vesuvius::VSTexture2D>(assetID);
											icon = thumbnail;
											std::filesystem::path pathstr = thumbnail->GetPath();
											std::filesystem::path path = pathstr;
											std::filesystem::path label = path.filename();
											buttonLabel = label.string();
										}

										CPImGui::FramedButtonWithIcon(name.c_str(), icon, buttonLabel.c_str());

										if (ImGui::BeginDragDropTarget())
										{
											if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
											{
												const wchar_t* path = (const wchar_t*)payload->Data;
												if (CPFileSystemUtils::IsTextureFile(path))
												{
													std::wstring wpath(path);
													std::string pathstr(wpath.begin(), wpath.end());
													CCUUID uuid = Cataclysm::CCAssetSystem::GetUUIDFromPath(pathstr);
													scriptField.SetValue(uuid);
												}
											}
											ImGui::EndDragDropTarget();
										}

										break;
									}
									case CCScriptFieldType::AudioClip:
									{
										CCUUID assetID = scriptField.GetValue<CCUUID>();
										std::string buttonLabel = "AudioClip (NULL)";
										if (assetID != 0)
										{
											std::filesystem::path pathstr = CCAssetSystem::GetAsset<CCAudioClip>(assetID)->GetFilePath();
											std::filesystem::path path = pathstr;
											std::filesystem::path label = path.filename();
											buttonLabel = label.string();
										}

										CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_AUDIO_CLIP, buttonLabel.c_str());

										if (ImGui::BeginDragDropTarget())
										{
											if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
											{
												const wchar_t* path = (const wchar_t*)payload->Data;
												if (CPFileSystemUtils::IsAudioFile(path))
												{
													std::wstring wpath(path);
													std::string pathstr(wpath.begin(), wpath.end());
													CCUUID uuid = Cataclysm::CCAssetSystem::GetUUIDFromPath(pathstr);
													scriptField.SetValue(uuid);
												}
											}
											ImGui::EndDragDropTarget();
										}

										break;
									}
									case CCScriptFieldType::Font:
									{
										CCUUID assetID = scriptField.GetValue<CCUUID>();
										std::string buttonLabel = "Font (NULL)";
										if (assetID != 0)
										{
											auto& asset = CCAssetSystem::GetAsset<Vesuvius::VSFont>(assetID);
											std::string filepath = asset->FilePath;
											// CC_CORE_WARN(filepath);

											std::filesystem::path path = filepath;
											std::filesystem::path label = path.filename();
											buttonLabel = label.string(); // "Font";
										}

										CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_FONT, buttonLabel.c_str());

										if (ImGui::BeginDragDropTarget())
										{
											if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
											{
												const wchar_t* path = (const wchar_t*)payload->Data;
												if (CPFileSystemUtils::IsFontFile(path))
												{
													std::wstring wpath(path);
													std::string pathstr(wpath.begin(), wpath.end());
													CCUUID uuid = Cataclysm::CCAssetSystem::GetUUIDFromPath(pathstr);
													scriptField.SetValue(uuid);
												}
											}
											ImGui::EndDragDropTarget();
										}

										break;
									}
									default:
									{
										break;
									}
								}
							}
							else
							{
								switch (field.Type)
								{
									case CCScriptFieldType::Float:
									{
										float data = 0.0f;
										if (CPImGui::FramedDragFloat(name.c_str(), &data))
										{
											CCScriptFieldInstance& fieldInstance = entityFields[name];
											fieldInstance.Field = field;
											fieldInstance.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::Bool:
									{
										bool data = false;
										if (CPImGui::FramedCheckbox(name.c_str(), &data))
										{
											CCScriptFieldInstance& fieldInstance = entityFields[name];
											fieldInstance.Field = field;
											fieldInstance.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::Int:
									{
										int data = 0;
										if (CPImGui::FramedDragInt(name.c_str(), &data))
										{
											CCScriptFieldInstance& fieldInstance = entityFields[name];
											fieldInstance.Field = field;
											fieldInstance.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::Double:
									{
										double data = 0.0;
										if (CPImGui::InputDouble(name.c_str(), &data))
										{
											CCScriptFieldInstance& fieldInstance = entityFields[name];
											fieldInstance.Field = field;
											fieldInstance.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::Vec2:
									{
										glm::vec2 data = glm::vec2(0.0f, 0.0f);
										const std::string& uniqueID = std::to_string(entity.GetUUID()) + "_MonoScript_" + name;
										if (CPImGui::DragVec2(name, uniqueID, data))
										{
											CCScriptFieldInstance& fieldInstance = entityFields[name];
											fieldInstance.Field = field;
											fieldInstance.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::Vec3:
									{
										glm::vec3 data = glm::vec3(0.0f, 0.0f, 0.0f);
										const std::string& uniqueID = std::to_string(entity.GetUUID()) + "_MonoScript_" + name;
										if (CPImGui::DragVec3(name, uniqueID, data))
										{
											CCScriptFieldInstance& fieldInstance = entityFields[name];
											fieldInstance.Field = field;
											fieldInstance.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::Vec4:
									{
										glm::vec4 data = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
										const std::string& uniqueID = std::to_string(entity.GetUUID()) + "_MonoScript_" + name;
										if (CPImGui::DragVec4(name, uniqueID, data))
										{
											CCScriptFieldInstance& fieldInstance = entityFields[name];
											fieldInstance.Field = field;
											fieldInstance.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::String:
									{
										std::string data = std::string();
										if (CPImGui::InputTextMultiline(name.c_str(), &data))
										{
											CCScriptFieldInstance& fieldInstance = entityFields[name];
											fieldInstance.Field = field;
											fieldInstance.SetValue(data);
										}
										break;
									}
									case CCScriptFieldType::Entity:
									{
										CCUUID entityUID = entityFields[name].GetValue<CCUUID>();
										Entity storedEntity = CCSceneEntityManager::GetEntityByUUID(scene.get(), entityUID);

										std::string buttonLabel = "Entity (Null)";
										if (storedEntity && storedEntity.HasComponent<TagComponent>())
										{
											buttonLabel = storedEntity.GetComponent<TagComponent>().Tag;
										}

										CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_ENTITY, buttonLabel.c_str());

										if (ImGui::BeginDragDropTarget())
										{
											if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP"))
											{
												CCUUID entityID = *(const CCUUID*)payload->Data;
												CCScriptFieldInstance& fieldInstance = entityFields[name];
												fieldInstance.Field = field;
												fieldInstance.SetValue(entityID);
											}
											ImGui::EndDragDropTarget();
										}
										ImGui::Columns(1);

										break;
									}
									case CCScriptFieldType::Texture2D:
									{
										CCUUID assetID = entityFields[name].GetValue<CCUUID>();
										std::string buttonLabel = "Texture2D (NULL)";
										if (assetID != 0)
										{
											std::string pathstr = CCAssetSystem::GetAsset<Vesuvius::VSTexture2D>(assetID)->GetPath();
											std::filesystem::path path = pathstr;
											std::filesystem::path label = path.filename();
											buttonLabel = label.string();
										}

										CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_TEXTURE_2D, buttonLabel.c_str());

										if (ImGui::BeginDragDropTarget())
										{
											if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
											{
												const wchar_t* path = (const wchar_t*)payload->Data;
												if (CPFileSystemUtils::IsTextureFile(path))
												{
													std::wstring wpath(path);
													std::string pathstr(wpath.begin(), wpath.end());
													CCUUID uuid = Cataclysm::CCAssetSystem::GetUUIDFromPath(pathstr);
													CCScriptFieldInstance& fieldInstance = entityFields[name];
													fieldInstance.Field = field;
													fieldInstance.SetValue(uuid);
												}
											}
											ImGui::EndDragDropTarget();
										}

										break;
									}
									case CCScriptFieldType::AudioClip:
									{
										CCUUID assetID = entityFields[name].GetValue<CCUUID>();
										std::string buttonLabel = "AudioClip (NULL)";
										if (assetID != 0)
											buttonLabel = CCAssetSystem::GetAsset<CCAudioClip>(assetID)->GetFilePath();

										CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_AUDIO_CLIP, buttonLabel.c_str());

										if (ImGui::BeginDragDropTarget())
										{
											if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
											{
												const wchar_t* path = (const wchar_t*)payload->Data;
												if (CPFileSystemUtils::IsAudioFile(path))
												{
													std::wstring wpath(path);
													std::string pathstr(wpath.begin(), wpath.end());
													CCUUID uuid = Cataclysm::CCAssetSystem::GetUUIDFromPath(pathstr);
													CCScriptFieldInstance& fieldInstance = entityFields[name];
													fieldInstance.Field = field;
													fieldInstance.SetValue(uuid);
												}
											}
											ImGui::EndDragDropTarget();
										}

										break;
									}
									case CCScriptFieldType::Font:
									{
										CCUUID assetID = entityFields[name].GetValue<CCUUID>();
										std::string buttonLabel = "Font (NULL)";
										if (assetID != 0)
											buttonLabel = CCAssetSystem::GetAsset<Vesuvius::VSFont>(assetID)->FilePath;

										CPImGui::FramedButtonWithIcon(name.c_str(), CPResources::ICON_FONT, buttonLabel.c_str());

										if (ImGui::BeginDragDropTarget())
										{
											if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
											{
												const wchar_t* path = (const wchar_t*)payload->Data;
												if (CPFileSystemUtils::IsFontFile(path))
												{
													std::wstring wpath(path);
													std::string pathstr(wpath.begin(), wpath.end());
													CCUUID uuid = Cataclysm::CCAssetSystem::GetUUIDFromPath(pathstr);
													CCScriptFieldInstance& fieldInstance = entityFields[name];
													fieldInstance.Field = field;
													fieldInstance.SetValue(uuid);
												}
											}
											ImGui::EndDragDropTarget();
										}

										break;
									}
									default:
									{
										break;
									}
								}
							}
						}
					}
				}
		});

		DrawComponent<SpriteRenderer>("SpriteRenderer", entity, [&](auto& component)
		{
			CPImGui::ColorSelect("Color", glm::value_ptr(component.Color));
		
			if (component.Texture && component.Texture->GetRendererID() != 0)
			{
				std::string path = component.Texture->GetPath();
				size_t pos = path.find_last_of("\\");
				path = (pos != std::string::npos) ? path.substr(pos + 1) : path;
				CPImGui::FramedButtonWithIcon("Texture2D", component.Texture, path.c_str());
			}
			else
			{
				CPImGui::FramedButtonWithIcon("Texture2D", CPResources::ICON_TEXTURE_2D, "NULL");
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath(path);

					if (CPFileSystemUtils::IsTextureFile(texturePath))
					{
						CCUUID assetID = Cataclysm::CCAssetSystem::GetUUIDFromPath(texturePath.string());
						auto& texture = Cataclysm::CCAssetSystem::GetAsset<Vesuvius::VSTexture2D>(assetID);
						if (texture->IsLoaded())
							component.Texture = texture;
					}
					else
					{
						CC_CORE_WARN("[CPSceneHierarchyPanel::DrawComponent<SpriteRenderer>] Texture must be a .png, .jpg, or .jpeg file!");
					}
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::Columns(1);

			CPImGui::FramedDragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);

			//if (component.Texture && component.Texture->GetRendererID() != 0)
			//{
			//	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
			//	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 1 });

			//	const float lineHeight = (ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f) * CPStyles::GetUIScale();
			//	const float spacing = 4.0f;

			//	if (ImGui::Button("Set Native Size", ImVec2(ImGui::GetContentRegionAvail().x - lineHeight - spacing - spacing, 0)))
			//	{
			//		auto& tc = entity.GetComponent<Transform>();
			//		glm::vec2 textureSize = { (float)component.Texture->GetWidth(), (float)component.Texture->GetHeight() };
			//		tc.Scale.x = textureSize.x;
			//		tc.Scale.y = textureSize.y;
			//	}

			//	ImGui::PopStyleVar(2);
			//}
		});

		DrawComponent<CircleRenderer>("CircleRenderer", entity, [](auto& component)
		{
			CPImGui::ColorSelect("Color", glm::value_ptr(component.Color));
			CPImGui::FramedDragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
			CPImGui::FramedDragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
		});

		DrawComponent<AudioSource>("AudioSource", entity, [this](auto& component)
		{
			if (!component.FilePath.empty())
			{
				std::string path = component.FilePath;
				size_t pos = path.find_last_of("\\");
				path = (pos != std::string::npos) ? path.substr(pos + 1) : path;

				CPImGui::FramedButtonWithIcon("AudioClip", CPResources::ICON_AUDIO_CLIP, path.c_str());
			}
			else
				CPImGui::FramedButtonWithIcon("AudioClip", CPResources::ICON_AUDIO_CLIP, "NULL");


			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = static_cast<const wchar_t*>(payload->Data);
					std::filesystem::path audioPath = path;

					if (CPFileSystemUtils::IsAudioFile(audioPath))
					{
						CCUUID assetID = Cataclysm::CCAssetSystem::GetUUIDFromPath(audioPath.string());
						auto& audioClip = Cataclysm::CCAssetSystem::GetAsset<CCAudioClip>(assetID);
						component.FilePath = audioPath.string();
						component.AudioClip = audioClip;
						if (component.AudioClip)
							component.AudioClip->SetLooping(component.Loop);
					}
					else
					{
						CC_CORE_WARN("[CPSceneHierarchyPanel::DrawComponent<AudioSource>] AudioClip must be a .wav, .mp3, or .ogg file!");
					}
				}
			}
			ImGui::Columns(1);

			CPImGui::FramedCheckbox("Loop", &component.Loop);
			CPImGui::FramedCheckbox("Play On Start", &component.PlayOnStart);

			if (CPImGui::FramedDragFloat("Volume", &component.Volume, 0.01f, 0.0f, 1.0f))
			{
				if (component.AudioClip && component.AudioClip->IsValid())
					component.AudioClip->SetVolume(component.Volume);
			}

			if (component.AudioClip)
			{
				if (!m_Context->IsRunning())
				{
					ImGui::Columns(2, nullptr, false);
					ImGui::SetColumnWidth(0, 100.0f);

					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, CPStyles::GetElementRounding());
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 1 });

					ImGui::NextColumn();

					if (CPImGui::ImageButton(CPResources::ICON_PLAY, CPStyles::GetIconSize())) CCAudioEngine::Play(component.AudioClip);
					ImGui::SameLine();

					if (CPImGui::ImageButton(CPResources::ICON_PAUSE, CPStyles::GetIconSize())) CCAudioEngine::Pause(component.AudioClip);
					ImGui::SameLine();

					if (CPImGui::ImageButton(CPResources::ICON_STOP, CPStyles::GetIconSize())) CCAudioEngine::Stop(component.AudioClip);

					ImGui::PopStyleVar(2);
					ImGui::Columns(1);
				}
			}
		});

		DrawComponent<Rigidbody2D>("Rigidbody2D", entity, [](auto& component)
		{
			const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
			const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
			if (CPImGui::BeginCombo("Body Type", currentBodyTypeString))
			{
				for (int i = 0; i <= 2; i++)
				{
					bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
					{
						currentBodyTypeString = bodyTypeStrings[i];
						component.Type = (Rigidbody2D::BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			ImGui::Columns(1);

			CPImGui::FramedCheckbox("Fixed Rotation", &component.FixedRotation);
			CPImGui::FramedDragFloat("Gravity Scale", &component.GravityScale, 1.0f, 0.0f, 1000.0f);
			CPImGui::FramedDragFloat("Mass", &component.Mass, 1.0f, 0.00001f, FLT_MAX);
		});

		DrawComponent<BoxCollider2D>("BoxCollider2D", entity, [=](auto& component)
		{
			CPImGui::DragVec2("Offset", std::to_string(id.ID) + "_BoxCollider2D_Offset", component.Offset);
			CPImGui::DragVec2("Size", std::to_string(id.ID) + "_BoxCollider2D_Size", component.Size);
			CPImGui::FramedDragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			CPImGui::FramedDragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			CPImGui::FramedDragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			CPImGui::FramedDragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			CPImGui::FramedCheckbox("IsTrigger", &component.IsTrigger);
		});

		DrawComponent<CircleCollider2D>("CircleCollider2D", entity, [=](auto& component)
		{
			CPImGui::DragVec2("Offset", std::to_string(id.ID) + "_CircleCollider2D_Offset", component.Offset);
			CPImGui::FramedDragFloat("Radius", &component.Radius);
			CPImGui::FramedDragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			CPImGui::FramedDragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			CPImGui::FramedDragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			CPImGui::FramedDragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			CPImGui::FramedCheckbox("IsTrigger", &component.IsTrigger);
		});

		DrawComponent<Text>("Text", entity, [](auto& component)
		{
			CPImGui::InputTextMultiline("Text", &component.TextString);

			std::filesystem::path fontpath = component.FontPath;
			std::string filename = fontpath.filename().string();
			CPImGui::FramedButtonWithIcon("Font", CPResources::ICON_FONT, filename.c_str());

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = static_cast<const wchar_t*>(payload->Data);
					std::filesystem::path fontPath = path;

					if (CPFileSystemUtils::IsFontFile(fontPath))
					{
						CCUUID assetID = Cataclysm::CCAssetSystem::GetUUIDFromPath(fontPath.string());
						auto& font = Cataclysm::CCAssetSystem::GetAsset<Vesuvius::VSFont>(assetID);
						component.FontPath = fontPath.string();
						component.FontAsset = font;
					}
					else
					{
						CC_CORE_WARN("[CPSceneHierarchyPanel::DrawComponent<AudioSource>] AudioClip must be a .wav, .mp3, or .ogg file!");
					}
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::Columns(1);

			CPImGui::ColorSelect("Color", glm::value_ptr(component.Color));
			CPImGui::FramedDragFloat("Kerning", &component.Kerning, 0.025f, -100, 100);
			CPImGui::FramedDragFloat("Leading", &component.LineSpacing, 0.025f, -100, 100);
		});
	}
}
