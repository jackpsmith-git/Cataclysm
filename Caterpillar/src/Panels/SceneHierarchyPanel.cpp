#include "SceneHierarchyPanel.h"
#include "Cataclysm/ECS/Components.h"

#include "Cataclysm/Scripting/ScriptEngine.h"
#include "Cataclysm/UI/UI.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <cstring>

#ifdef _MSVC_LANG
	#define _CRT_SECURE_NO_WARNINGS
#endif
#include <regex>

namespace Cataclysm
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		if (ImGui::Button("+", ImVec2(ImGui::GetContentRegionAvail().x, 25)))
		{
			m_Context->CreateEntity("Empty Entity");
		}

		auto view = m_Context->m_Registry.view<IDComponent, TagComponent>();

		for (auto e : view)
		{
			Entity entity = { e, m_Context.get() };
			UUID id = entity.GetUUID();

			if (m_Context->GetParent(id) == 0)
				DrawEntityNode(entity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		ImVec2 dropAreaSize = ImGui::GetContentRegionAvail();
		ImGui::InvisibleButton("HierarchyDropTarget", dropAreaSize);

		// Optional: handle dropping entities onto empty space to unparent
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP"))
			{
				UUID droppedID = *(const UUID*)payload->Data;
				m_Context->RemoveParent(droppedID);
			}
			ImGui::EndDragDropTarget();
		}

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("New Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		/*
		if (m_Context)
		{
			m_Context->m_Registry.each([&](auto entityID)
				{
					Entity entity{ entityID , m_Context.get() };
					DrawEntityNode(entity);
				});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("New Entity"))
					m_Context->CreateEntity("Empty Entity");

				ImGui::EndPopup();
			}
		}
		*/

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		bool hasChildren = !m_Context->GetChildren(entity.GetUUID()).empty();
		if (!hasChildren)
			flags |= ImGuiTreeNodeFlags_Leaf;

		if (!m_Context->IsEnabled(entity))
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (!m_Context->IsEnabled(entity))
			ImGui::PopStyleColor();

		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			m_SelectionContext = entity;
		}

		// === DRAG SOURCE ===
		if (ImGui::BeginDragDropSource())
		{
			UUID id = entity.GetUUID();
			ImGui::SetDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP", &id, sizeof(UUID));
			ImGui::Text("%s", tag.c_str());
			ImGui::EndDragDropSource();
		}

		// === DROP TARGET ===
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP"))
			{
				UUID droppedID = *(const UUID*)payload->Data;

				if (droppedID != entity.GetUUID()) // prevent self-parenting
				{
					m_Context->SetParent(droppedID, entity.GetUUID());
				}
			}
			ImGui::EndDragDropTarget();
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			for (UUID childID : m_Context->GetChildren(entity.GetUUID()))
			{
				Entity child = m_Context->GetEntityByUUID(childID);
				DrawEntityNode(child);
			}

			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}

		/*
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		if (!entity.GetComponent<IDComponent>().Enabled)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (!entity.GetComponent<IDComponent>().Enabled)
			ImGui::PopStyleColor();

		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			m_SelectionContext = entity;
		}

		Entity* entityPtr = &entity;

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("ENTITY_HIERARCHY_ITEM", entityPtr, sizeof(Entity*));
			ImGui::EndDragDropSource();
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			//ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			//bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			//if (opened)
			//	ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
		*/
	}

	static bool DrawVec2Control(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		bool edited = false;
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0 , 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f"))
			edited = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f"))
			edited = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
		return edited;
	}

	static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		bool edited = false;

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0 , 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f"))
			edited = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f"))
			edited = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f"))
			edited = true;
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
		return edited;
	}

	static bool DrawVec4Control(const std::string& label, glm::vec4& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		bool edited = false;

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0 , 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f"))
			edited = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f"))
			edited = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f"))
			edited = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("W", buttonSize))
			values.w = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##W", &values.w, 0.1f, 0.0f, 0.0f, "%.2f"))
			edited = true;
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
		return edited;
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];

			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			ImGui::PushFont(boldFont);
			if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopFont();
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	static std::string ParseCSharpClassFullName(const std::filesystem::path& filePath)
	{
		std::ifstream file(filePath);
		if (!file.is_open())
			return "";

		std::string line;
		std::string namespaceName;
		std::string className;

		std::regex namespaceRegex(R"(\s*namespace\s+([a-zA-Z0-9_.]+))");
		std::regex classRegex(R"(\s*(public\s+)?(class|struct)\s+([a-zA-Z0-9_]+))");

		while (std::getline(file, line))
		{
			std::smatch match;
			if (namespaceName.empty() && std::regex_search(line, match, namespaceRegex))
			{
				namespaceName = match[1].str();
			}
			else if (className.empty() && std::regex_search(line, match, classRegex))
			{
				className = match[3].str();
			}

			if (!namespaceName.empty() && !className.empty())
				break;
		}

		file.close();

		if (!className.empty())
		{
			if (!namespaceName.empty())
				return namespaceName + "." + className;
			else
				return className;
		}

		return "";
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		auto& id = entity.GetComponent<IDComponent>();
		ImGui::Checkbox(" ", &id.Enabled);
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
		ImGui::PushItemWidth(-1);

		ImGui::PushFont(boldFont);
		if (ImGui::Button(" + "))
			ImGui::OpenPopup("AddComponent");
		ImGui::PopFont();

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<MonoScriptComponent>("MonoScript");
			DisplayAddComponentEntry<SpriteRendererComponent>("SpriteRenderer");
			DisplayAddComponentEntry<CircleRendererComponent>("CircleRenderer");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("BoxCollider2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("CircleCollider2D");
			DisplayAddComponentEntry<TextComponent>("Text");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		uint64_t uuid = (uint64_t)entity.GetUUID();
		std::stringstream ss;
		ss << "UUID: " << uuid;
		ImGui::Text(ss.str().c_str());

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale);

			ImGui::Spacing();
			if (ImGui::Button("Reset", ImVec2(ImGui::GetContentRegionMax().x, 20.0f)))
				component.Reset();
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
				auto& camera = component.Camera;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float fov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("FOV", &fov))
						camera.SetPerspectiveVerticalFOV(glm::radians(fov));

					float perspNearClip = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &perspNearClip))
						camera.SetPerspectiveNearClip(perspNearClip);

					float perspFarClip = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &perspFarClip))
						camera.SetPerspectiveFarClip(perspFarClip);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("FOV", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNearClip = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &orthoNearClip))
						camera.SetOrthographicNearClip(orthoNearClip);

					float orthoFarClip = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &orthoFarClip))
						camera.SetOrthographicFarClip(orthoFarClip);

					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}

				ImGui::Spacing();
				if (ImGui::Button("Reset", ImVec2(ImGui::GetContentRegionMax().x, 20.0f)))
					component.Reset();
		});

		DrawComponent<MonoScriptComponent>("MonoScript", entity, [entity, scene = m_Context](auto& component) mutable
		{
				bool scriptClassExists = ScriptEngine::EntityClassExists(component.ClassName);

				static char buffer[64];
				strcpy_s(buffer, sizeof(buffer), component.ClassName.c_str());

				UI::ScopedStyleColor textColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f), !scriptClassExists);

				if (scriptClassExists)
				{
					ImGui::Button(component.ClassName.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 25));
				}
				else
				{
					ImGui::Button("MonoScript", ImVec2(ImGui::GetContentRegionAvail().x, 25));
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = static_cast<const wchar_t*>(payload->Data);

						// Convert wide string path to std::string
						std::filesystem::path scriptPath = path;
						std::string filename = scriptPath.filename().string(); // e.g., MyScript.cs

						std::string className = filename.substr(0, filename.find_last_of('.'));
						component.ClassName = className;

						std::string fullClassName = ParseCSharpClassFullName(scriptPath);
						if (!fullClassName.empty())
							component.ClassName = fullClassName;
					}

					ImGui::EndDragDropTarget();
				}



				/*
				if (ImGui::InputText("Class", buffer, sizeof(buffer)))
				{
					component.ClassName = buffer;
					return;
				}
				*/

				// Fields
				bool sceneRunning = scene->IsRunning();
				if (sceneRunning)
				{
					Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity.GetUUID());
					if (scriptInstance)
					{
						const auto& fields = scriptInstance->GetScriptClass()->GetFields();

						for (const auto& [name, field] : fields)
						{
							if (field.Type == ScriptFieldType::Float)
							{
								float data = scriptInstance->GetFieldValue<float>(name);
								if (ImGui::DragFloat(name.c_str(), &data))
								{
									scriptInstance->SetFieldValue(name, data);
								}
							}
							else if (field.Type == ScriptFieldType::Bool)
							{
								bool data = scriptInstance->GetFieldValue<bool>(name);
								if (ImGui::Checkbox(name.c_str(), &data))
								{
									scriptInstance->SetFieldValue(name, data);
								}
							}
							else if (field.Type == ScriptFieldType::Int)
							{
								int data = scriptInstance->GetFieldValue<int>(name);
								if (ImGui::DragInt(name.c_str(), &data))
								{
									scriptInstance->SetFieldValue(name, data);
								}
							}
							else if (field.Type == ScriptFieldType::Double)
							{
								double data = scriptInstance->GetFieldValue<double>(name);
								if (ImGui::InputDouble(name.c_str(), &data))
								{
									scriptInstance->SetFieldValue(name, data);
								}
							}
							else if (field.Type == ScriptFieldType::Vec2)
							{
								glm::vec2 data = scriptInstance->GetFieldValue<glm::vec2>(name);
								if (DrawVec2Control(name, data))
								{
									scriptInstance->SetFieldValue(name, data);
								}
							}
							else if (field.Type == ScriptFieldType::Vec3)
							{
								glm::vec3 data = scriptInstance->GetFieldValue<glm::vec3>(name);
								if (DrawVec3Control(name, data))
								{
									scriptInstance->SetFieldValue(name, data);
								}
							}
							else if (field.Type == ScriptFieldType::Vec4)
							{
								glm::vec4 data = scriptInstance->GetFieldValue<glm::vec4>(name);
								if (DrawVec4Control(name, data))
								{
									scriptInstance->SetFieldValue(name, data);
								}
							}
							else if (field.Type == ScriptFieldType::Entity)
							{
								Entity data = scriptInstance->GetFieldValue<Entity>(name);

								ImGui::Button(name.c_str(), ImVec2(100.0f, 0.0f));
								if (ImGui::BeginDragDropTarget())
								{
									if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP"))
									{
										Entity* entity = (Entity*)payload->Data;

										std::stringstream warning;
										warning << "[SceneHierarchyPanel::DrawComponent<MonoScriptComponent>] " << *(UUID*)payload->Data;
										CC_CORE_WARN(warning.str());
										scriptInstance->SetFieldValue(name, entity);
									}
									ImGui::EndDragDropTarget();
								}

							}
						}
					}
				}
				else
				{
					if (scriptClassExists)
					{
						Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(component.ClassName);
						const auto& fields = entityClass->GetFields();

						auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
						
						for (const auto& [name, field] : fields)
						{
							if (entityFields.find(name) != entityFields.end())
							{
								ScriptFieldInstance& scriptField = entityFields.at(name);

								if (field.Type == ScriptFieldType::Float)
								{
									float data = scriptField.GetValue<float>();
									if (ImGui::DragFloat(name.c_str(), &data))
										scriptField.SetValue(data);
								}
								else if (field.Type == ScriptFieldType::Bool)
								{
									bool data = scriptField.GetValue<bool>();
									if (ImGui::Checkbox(name.c_str(), &data))
										scriptField.SetValue(data);
								}
								else if (field.Type == ScriptFieldType::Int)
								{
									int data = scriptField.GetValue<int>();
									if (ImGui::DragInt(name.c_str(), &data))
									{
										scriptField.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Double)
								{
									double data = scriptField.GetValue<double>();
									if (ImGui::InputDouble(name.c_str(), &data))
									{
										scriptField.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Vec2)
								{
									glm::vec2 data = scriptField.GetValue<glm::vec2>();
									if (DrawVec2Control(name, data))
									{
										scriptField.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Vec3)
								{
									glm::vec3 data = scriptField.GetValue<glm::vec3>();
									if (DrawVec3Control(name, data))
									{
										scriptField.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Vec4)
								{
									glm::vec4 data = scriptField.GetValue<glm::vec4>();
									if (DrawVec4Control(name, data))
									{
										scriptField.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Entity)
								{
									Entity data = scriptField.GetValue<Entity>();

									ImGui::Button(name.c_str(), ImVec2(100.0f, 0.0f));
									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP"))
										{
											Entity* entity = (Entity*)payload->Data;
											scriptField.SetValue(entity);
										}
										ImGui::EndDragDropTarget();
									}
								}
							}
							else
							{
								if (field.Type == ScriptFieldType::Float)
								{
									float data = 0.0f;
									if (ImGui::DragFloat(name.c_str(), &data))
									{
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;

										fieldInstance.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Bool)
								{
									bool data = false;
									if (ImGui::Checkbox(name.c_str(), &data))
									{
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;

										fieldInstance.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Int)
								{
									int data = 0;
									if (ImGui::DragInt(name.c_str(), &data))
									{
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Double)
								{
									double data = 0.0;
									if (ImGui::InputDouble(name.c_str(), &data))
									{
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Vec2)
								{
									glm::vec2 data = glm::vec2(0.0f, 0.0f);
									if (DrawVec2Control(name, data))
									{
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Vec3)
								{
									glm::vec3 data = glm::vec3(0.0f, 0.0f, 0.0f);
									if (DrawVec3Control(name, data))
									{
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Vec4)
								{
									glm::vec4 data = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
									if (DrawVec4Control(name, data))
									{
										ScriptFieldInstance& fieldInstance = entityFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Entity)
								{
									ImGui::Button(name.c_str(), ImVec2(100.0f, 0.0f));
									if (ImGui::BeginDragDropTarget())
									{
										if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY_DRAG_DROP"))
										{
											Entity* data = (Entity*)payload->Data;
											ScriptFieldInstance& fieldInstance = entityFields[name];
											fieldInstance.Field = field;
											fieldInstance.SetValue(data);
										}
										ImGui::EndDragDropTarget();
									}

								}
							}
						}
					}
				}
		});

		DrawComponent<SpriteRendererComponent>("SpriteRenderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		
			if (component.Texture && component.Texture->GetRendererID() != 0)
			{
				float thumbnailSize = 22.0f;
				ImGui::Image(
					(void*)(uintptr_t)component.Texture->GetRendererID(),
					{ thumbnailSize, thumbnailSize },
					{ 0, 1 }, { 1, 0 });
				ImGui::SameLine();

				std::string path = component.Texture->GetPath();
				size_t pos = path.find_last_of("\\");
				path = (pos != std::string::npos) ? path.substr(pos + 1) : path;
				ImGui::Button(path.c_str(), ImVec2(ImGui::GetContentRegionMax().x - 50.0f, 0.0f));
			}
			else
			{
				ImGui::Button("Texture", ImVec2(ImGui::GetContentRegionMax().x - 50.0f, 0.0f));
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath(path);
					Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
					if (texture->IsLoaded())
					{
						component.Texture = texture;
					}
					else
					{
						CC_WARN("Could not load texture {0}", texturePath.filename().string());
					}
				}
				ImGui::EndDragDropTarget();
			}


			ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);

			ImGui::Spacing();
			if (ImGui::Button("Reset", ImVec2(ImGui::GetContentRegionMax().x, 20.0f)))
				component.Reset();
		});

		DrawComponent<CircleRendererComponent>("CircleRenderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
			ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);

			ImGui::Spacing();
			if (ImGui::Button("Reset", ImVec2(ImGui::GetContentRegionMax().x, 20.0f)))
				component.Reset();
		});

		DrawComponent<Rigidbody2DComponent>("Rigidbody2D", entity, [](auto& component)
		{
			const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
			const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
			if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
			{
				for (int i = 0; i <= 2; i++)
				{
					bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
					{
						currentBodyTypeString = bodyTypeStrings[i];
						component.Type = (Rigidbody2DComponent::BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
			ImGui::DragFloat("Gravity Scale", &component.GravityScale, 1.0f, 0.0f, 1000.0f);
			ImGui::DragFloat("Mass", &component.Mass, 1.0f, 0.00001f, FLT_MAX);

			ImGui::Spacing();
			if (ImGui::Button("Reset", ImVec2(ImGui::GetContentRegionMax().x, 20.0f)))
				component.Reset();
		});

		DrawComponent<BoxCollider2DComponent>("BoxCollider2D", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			ImGui::Checkbox("IsTrigger", &component.IsTrigger);

			ImGui::Spacing();
			if (ImGui::Button("Reset", ImVec2(ImGui::GetContentRegionMax().x, 20.0f)))
				component.Reset();
		});

		DrawComponent<CircleCollider2DComponent>("CircleCollider2D", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat("Radius", &component.Radius);
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			ImGui::Checkbox("IsTrigger", &component.IsTrigger);

			ImGui::Spacing();
			if (ImGui::Button("Reset", ImVec2(ImGui::GetContentRegionMax().x, 20.0f)))
				component.Reset();
		});

		DrawComponent<TextComponent>("Text", entity, [](auto& component)
		{
			ImGui::InputTextMultiline("Text", &component.TextString);
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Kerning", &component.Kerning, 0.025f);
			ImGui::DragFloat("Leading", &component.LineSpacing, 0.025f);

			float thumbnailSize = 22.0f;
			TextComponent tc = component;
			Cataclysm::Ref<Texture2D> atlasTexture = tc.FontAsset->GetAtlasTexture();

			ImGui::Image(
				(ImTextureID)(uint64_t)atlasTexture->GetRendererID(),
				{ thumbnailSize, thumbnailSize },
				{ 0, 1 }, { 1, 0 });
			ImGui::SameLine();

			std::filesystem::path fontpath = component.FontPath;
			std::string filename = fontpath.filename().string();
			ImGui::Button(filename.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 25));

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = static_cast<const wchar_t*>(payload->Data);
					std::filesystem::path fontPath = path;

					if (fontPath.extension() == ".ttf")
					{
						component.FontPath = fontPath.string();
						component.FontAsset = Font::GetFont(component.FontPath); // Load the font
					}
					else
					{
						CC_CORE_ERROR("[SceneHierarchyPanel::DrawComponents] Font must be a .ttf file!");
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::Spacing();
			if (ImGui::Button("Reset", ImVec2(ImGui::GetContentRegionMax().x, 20.0f)))
				component.Reset();
		});
	}
}
