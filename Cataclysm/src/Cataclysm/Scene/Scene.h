#pragma once

#include "Cataclysm/Core/Timestep.h"
#include "Cataclysm/Core/UUID.h"
#include "Cataclysm/Renderer/EditorCamera.h"

#include "entt.hpp"

class b2World;

namespace Cataclysm
{
	class Entity;
	class ContactListener2D;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnSimulationStart();
		void OnSimulationStop();

		void RuntimeErrorHit(std::string& error);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateSimulation(Timestep ts, EditorCamera& camera);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);
		Entity DuplicateEntity(Entity entity);

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUUID(UUID uuid);

		glm::mat4 GetWorldTransform(UUID entityID);
		void SetWorldTransform(UUID entityID, const glm::mat4& worldTransform);

		glm::mat4 ComposeTransform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);

		void OnRenderRuntime();

		void SetParent(UUID entity, UUID parent);
		void SetParentOnLoad(UUID entity, UUID parent);
		void RemoveParent(UUID entity);
		bool Scene::IsEnabled(Entity entity);

		UUID GetParent(UUID entity);
		std::vector<UUID> GetChildren(UUID entity);

		void Scene::UpdateTransformRecursive(Entity entity);

		Entity GetPrimaryCameraEntity();

		bool IsRunning() const { return m_IsRunning; }
		bool IsPaused() const { return m_IsPaused; }

		void SetPaused(bool paused) { m_IsPaused = paused; }

		void Step(int frames = 1);

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

		std::unordered_map<UUID, UUID> GetEntityParentMap() { return m_EntityParentMap; }
		std::unordered_multimap<UUID, UUID> GetEntityChildrenMap() { return m_EntityChildrenMap; }
		void RenderScene(EditorCamera& camera);
		void RenderScene(const Camera& camera, const glm::mat4& cameraTransform);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		void OnPhysics2DStart();
		void OnPhysics2DStop();

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;

		b2World* m_PhysicsWorld = nullptr;
		std::unique_ptr<ContactListener2D> m_ContactListener;

		std::unordered_map<UUID, entt::entity> m_EntityMap;
		std::unordered_map<UUID, UUID> m_EntityParentMap;
		std::unordered_multimap<UUID, UUID> m_EntityChildrenMap;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
