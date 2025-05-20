#pragma once

#include "Cataclysm.h"
#include "Cataclysm/Core/Core.h"
#include "Cataclysm/Core/Log.h"
#include "Cataclysm/Scene/Scene.h"
#include "Cataclysm/ECS/Entity.h"
#include "Cataclysm/Renderer/Texture.h"

namespace Cataclysm
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		SceneHierarchyPanel(const Cataclysm::Ref<Cataclysm::Scene>& context);

		void SetContext(const Cataclysm::Ref<Cataclysm::Scene>& context);

		void OnImGuiRender();

		Cataclysm::Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Cataclysm::Entity entity);
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		template<typename T, typename UIFunction>
		void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction);

		void DrawEntityNode(Cataclysm::Entity entity);
		void DrawComponents(Cataclysm::Entity entity);
	private:
		Cataclysm::Ref<Cataclysm::Scene> m_Context;
		Cataclysm::Entity m_SelectionContext;

		Cataclysm::Ref<Cataclysm::Texture2D> m_AudioSourceIcon;
		Cataclysm::Ref<Cataclysm::Texture2D> m_BoxColliderIcon;
		Cataclysm::Ref<Cataclysm::Texture2D> m_CircleColliderIcon;
		Cataclysm::Ref<Cataclysm::Texture2D> m_CircleRendererIcon;
		Cataclysm::Ref<Cataclysm::Texture2D> m_EntityIcon;
		Cataclysm::Ref<Cataclysm::Texture2D> m_RigidbodyIcon;
		Cataclysm::Ref<Cataclysm::Texture2D> m_ScriptIcon;
		Cataclysm::Ref<Cataclysm::Texture2D> m_SpriteRendererIcon;
		Cataclysm::Ref<Cataclysm::Texture2D> m_TextIcon;
		Cataclysm::Ref<Cataclysm::Texture2D> m_TransformIcon;
		Cataclysm::Ref<Cataclysm::Texture2D> m_CameraIcon;
	};
}
