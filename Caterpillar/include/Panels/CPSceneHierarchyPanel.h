#pragma once

#include "Cataclysm.h"
#include "Cataclysm/Core/CCCore.h"
#include "Cataclysm/Core/CCLog.h"
#include "Cataclysm/Scene/CCScene.h"
#include "Cataclysm/ECS/CCEntity.h"

#include "Cataclysm/Vesuvius/VSTexture.h"

namespace Cataclysm
{
	class CPSceneHierarchyPanel
	{
	public:
		CPSceneHierarchyPanel();
		CPSceneHierarchyPanel(const Cataclysm::CCRef<Cataclysm::CCScene>& context);

		void SetContext(const Cataclysm::CCRef<Cataclysm::CCScene>& context);
		void OnImGuiRender();

		Cataclysm::Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Cataclysm::Entity entity);
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		template<typename T, typename UIFunction>
		void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction);

		void DrawComponents(Cataclysm::Entity entity);
	private:
		Cataclysm::CCRef<Cataclysm::CCScene> m_Context;
		Cataclysm::Entity m_SelectionContext;
	};
}
