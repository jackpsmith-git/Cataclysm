#pragma once

#include "Cataclysm.h"
#include "Cataclysm/Core/Core.h"
#include "Cataclysm/Core/Log.h"
#include "Cataclysm/Scene/Scene.h"
#include "Cataclysm/ECS/Entity.h"

namespace Cataclysm
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Cataclysm::Ref<Cataclysm::Scene>& context);

		void SetContext(const Cataclysm::Ref<Cataclysm::Scene>& context);

		void OnImGuiRender();

		Cataclysm::Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Cataclysm::Entity entity);
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Cataclysm::Entity entity);
		void DrawComponents(Cataclysm::Entity entity);
	private:
		Cataclysm::Ref<Cataclysm::Scene> m_Context;
		Cataclysm::Entity m_SelectionContext;
	};
}
