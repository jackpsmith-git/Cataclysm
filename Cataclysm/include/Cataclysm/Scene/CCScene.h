#pragma once

#include "Cataclysm/Core/CCTimestep.h"
#include "Cataclysm/Core/CCUUID.h"
#include "Cataclysm/Vesuvius/VSEditorCamera.h"

#include "entt.hpp"

class b2World;

namespace Cataclysm
{
	class Entity;
	class CCContactListener2D;

	class CCScene
	{
	public:
		CCScene();
		~CCScene();

		static CCRef<CCScene> Copy(CCRef<CCScene> other);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void UpdateSceneRuntime(CCTimestep ts, Vesuvius::VSEditorCamera& camera);
		void UpdateSceneEditor(Vesuvius::VSEditorCamera& camera);
		void UpdateGameRuntime();

		void RuntimeErrorHit(std::string& error);

		void OnViewportResize(uint32_t width, uint32_t height);

		bool IsRunning() const { return m_IsRunning; }
		bool IsPaused() const { return m_IsPaused; }
		void SetPaused(bool paused) { m_IsPaused = paused; }

		inline void Step(int frames = 1) { m_StepFrames = frames; }

		std::unordered_map<CCUUID, CCUUID>& GetEntityParentMap() { return m_EntityParentMap; }
		std::unordered_multimap<CCUUID, CCUUID>& GetEntityChildrenMap() { return m_EntityChildrenMap; }
		std::unordered_map<CCUUID, entt::entity>& GetEntityMap() { return m_EntityMap; }
		entt::registry& GetRegistry() { return m_Registry; }
		void SetPhysicsWorld(b2World* world) { m_PhysicsWorld = world; }
		b2World* GetPhysicsWorld() { return m_PhysicsWorld; }

		inline void ClearPhysicsWorld()
		{
			delete m_PhysicsWorld;
			m_PhysicsWorld = nullptr;
		}

		std::unique_ptr<CCContactListener2D> m_ContactListener;
		glm::vec4& SkyboxColor = glm::vec4(0, 0, 0, 1);
		std::string& Name = std::string("NewScene");

		inline bool Stepped() { return m_StepFrames-- > 0; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		void RenderGame();
		void RenderScene(Vesuvius::VSEditorCamera& camera);
		void RenderElements();
		void RenderSprites();
		void RenderCircles();
		void RenderText();
	private:
		b2World* m_PhysicsWorld = nullptr;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;

		entt::registry m_Registry;
		std::unordered_map<CCUUID, entt::entity> m_EntityMap;
		std::unordered_map<CCUUID, CCUUID> m_EntityParentMap;
		std::unordered_multimap<CCUUID, CCUUID> m_EntityChildrenMap;

		friend class Entity;
		friend class CCSceneSerializer;
		friend class CPSceneHierarchyPanel;
	};
}
