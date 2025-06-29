#include "ccpch.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"
#include "Cataclysm/Math/CCMath.h"

#include "Cataclysm/Audio/CCAudioEngine.h"

namespace Cataclysm
{
	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<CCUUID, entt::entity>& enttMap)
	{
		([&]()
			{
				auto view = src.view<Component>();
				for (auto srcEntity : view)
				{
					entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);
					auto& srcComponent = src.get<Component>(srcEntity);
					dst.emplace_or_replace<Component>(dstEntity, srcComponent);
				}
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<CCUUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
			{
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Entity CCSceneEntityManager::CreateEntity(CCScene* scene, const std::string& name)
	{
		return CCSceneEntityManager::CreateEntityWithUUID(scene, CCUUID(), name);
	}

	Entity CCSceneEntityManager::CreateEntityWithUUID(CCScene* scene, CCUUID uuid, const std::string& name)
	{
		Entity entity = { scene->GetRegistry().create(), scene};
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<Transform>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		scene->GetEntityMap()[uuid] = entity;

		return entity;
	}

	void CCSceneEntityManager::DestroyEntity(CCScene* scene, Entity entity)
	{
		CCUUID id = entity.GetUUID();

		auto parentIt = scene->GetEntityParentMap().find(id);
		if (parentIt != scene->GetEntityParentMap().end())
		{
			CCUUID parentID = parentIt->second;

			auto [begin, end] = scene->GetEntityChildrenMap().equal_range(parentID);
			for (auto it = begin; it != end; ++it)
			{
				if (it->second == id)
				{
					scene->GetEntityChildrenMap().erase(it);
					break;
				}
			}

			scene->GetEntityParentMap().erase(parentIt);
		}

		auto [childBegin, childEnd] = scene->GetEntityChildrenMap().equal_range(id);
		std::vector<CCUUID> childrenToRemove;
		for (auto it = childBegin; it != childEnd; ++it)
		{
			childrenToRemove.push_back(it->second);
		}

		for (CCUUID childID : childrenToRemove)
		{
			scene->GetEntityParentMap().erase(childID);
			DestroyEntity(scene, GetEntityByUUID(scene, childID));
		}

		scene->GetEntityChildrenMap().erase(id);
		scene->GetRegistry().destroy(entity);
	}

	Entity CCSceneEntityManager::GetEntityByUUID(CCScene* scene, CCUUID uuid)
	{
		if (scene->GetEntityMap().find(uuid) != scene->GetEntityMap().end())
			return { scene->GetEntityMap().at(uuid), scene };

		return {};
	}

	Entity CCSceneEntityManager::FindEntityByName(CCScene* scene, std::string_view name)
	{
		auto view = scene->GetRegistry().view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, scene };
		}
		return {};
	}

	Entity CCSceneEntityManager::DuplicateEntity(CCScene* scene, Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CCSceneEntityManager::CreateEntity(scene, name);

		newEntity.GetComponent<IDComponent>().Enabled = entity.GetComponent<IDComponent>().Enabled;
		CopyComponentIfExists(AllComponents{}, newEntity, entity);

		if (GetParent(scene, entity.GetComponent<IDComponent>().ID) != 0)
		{
			SetParent(scene, newEntity.GetComponent<IDComponent>().ID, GetParent(scene, entity.GetComponent<IDComponent>().ID));
		}

		auto [begin, end] = scene->GetEntityChildrenMap().equal_range(entity.GetComponent<IDComponent>().ID);
		for (auto it = begin; it != end; ++it)
		{
			CCUUID childID = it->second;
			Entity childEntity = CCSceneEntityManager::GetEntityByUUID(scene, childID);

			if (!childEntity || !childEntity.HasComponent<IDComponent>())
				continue;

			Entity duplicatedChild = DuplicateEntity(scene, childEntity);

			if (GetParent(scene, entity.GetComponent<IDComponent>().ID) != 0)
				SetParent(scene, duplicatedChild.GetComponent<IDComponent>().ID, newEntity.GetComponent<IDComponent>().ID);
		}

		return newEntity;
	}

	bool CCSceneEntityManager::IsEnabled(CCScene* scene, Entity entity)
	{
		if (!entity.GetComponent<IDComponent>().Enabled)
		{
			return false;
		}
		else if (GetParent(scene, entity.GetUUID()) != 0)
		{
			CCUUID id = entity.GetUUID();
			CCUUID parentID = GetParent(scene, id);
			Entity parent = GetEntityByUUID(scene, parentID);
			return IsEnabled(scene, parent);
		}
		else
		{
			return true;
		}
	}

	CCUUID CCSceneEntityManager::GetParent(CCScene* scene, CCUUID entity)
	{
		auto it = scene->GetEntityParentMap().find(entity);
		return it != scene->GetEntityParentMap().end() ? it->second : 0;
	}

	void CCSceneEntityManager::SetParent(CCScene* scene, CCUUID entity, CCUUID parent)
	{
		if (entity == parent)
		{
			std::string error = "[CCScene::SetParent] Entity cannot be its own parent.";
			scene->RuntimeErrorHit(error);
			return;
		}

		CCUUID current = parent;
		while (current != 0)
		{
			if (current == entity)
			{
				std::string error = "[CCScene::SetParent] Entity's parent cannot be one of its descendants.";
				scene->RuntimeErrorHit(error);
				return;
			}
			current = GetParent(scene, current); // Traverse up the hierarchy
		}

		glm::mat4 oldGlobalTransform = CCSceneEntityManager::GetEntityByUUID(scene, entity).GetComponent<Transform>().GlobalTransform;

		RemoveParent(scene, entity);
		scene->GetEntityParentMap()[entity] = parent;
		scene->GetEntityChildrenMap().emplace(parent, entity);

		glm::mat4 parentGlobalTransform = CCSceneEntityManager::GetEntityByUUID(scene, parent).GetComponent<Transform>().GlobalTransform;
		glm::mat4 newLocalTransform = glm::inverse(parentGlobalTransform) * oldGlobalTransform;

		glm::vec3 translation, rotation, scale;
		Math::DecomposeTransform(newLocalTransform, translation, rotation, scale);

		Transform& tc = CCSceneEntityManager::GetEntityByUUID(scene, entity).GetComponent<Transform>();
		tc.Translation = translation;
		tc.Rotation = rotation;
		tc.Scale = scale;
	}

	void CCSceneEntityManager::SetParentOnLoad(CCScene* scene, CCUUID entity, CCUUID parent)
	{
		if (entity == parent)
		{
			std::string error = "[CCScene::SetParent] Entity cannot be its own parent.";
			scene->RuntimeErrorHit(error);
			return;
		}

		CCUUID current = parent;
		while (current != 0)
		{
			if (current == entity)
			{
				std::string error = "[CCScene::SetParent] Entity's parent cannot be one of its descendants.";
				scene->RuntimeErrorHit(error);
				return;
			}
			current = GetParent(scene, current);
		}

		RemoveParent(scene, entity);
		scene->GetEntityParentMap()[entity] = parent;
		scene->GetEntityChildrenMap().emplace(parent, entity);
	}

	void CCSceneEntityManager::RemoveParent(CCScene* scene, CCUUID entity)
	{
		auto it = scene->GetEntityParentMap().find(entity);
		if (it != scene->GetEntityParentMap().end())
		{
			glm::mat4 globalTransform = CCSceneEntityManager::GetEntityByUUID(scene, entity).GetComponent<Transform>().GlobalTransform;

			CCUUID oldParent = it->second;
			scene->GetEntityParentMap().erase(it);

			auto range = scene->GetEntityChildrenMap().equal_range(oldParent);
			for (auto i = range.first; i != range.second; ++i)
			{
				if (i->second == entity)
				{
					scene->GetEntityChildrenMap().erase(i);
					break;
				}
			}

			glm::vec3 translation, rotation, scale;
			Math::DecomposeTransform(globalTransform, translation, rotation, scale);

			Transform& tc = CCSceneEntityManager::GetEntityByUUID(scene, entity).GetComponent<Transform>();
			tc.Translation = translation;
			tc.Rotation = rotation;
			tc.Scale = scale;
		}

	}

	std::vector<CCUUID> CCSceneEntityManager::GetChildren(CCScene* scene, CCUUID entity)
	{
		std::vector<CCUUID> result;
		auto range = scene->GetEntityChildrenMap().equal_range(entity);
		for (auto it = range.first; it != range.second; ++it)
			result.push_back(it->second);
		return result;
	}

	glm::mat4 CCSceneEntityManager::GetWorldTransform(CCScene* scene, CCUUID entityID)
	{
		Entity entity = GetEntityByUUID(scene, entityID);
		const auto& transform = entity.GetComponent<Transform>();

		glm::mat4 local = glm::translate(glm::mat4(1.0f), transform.Translation)
			* glm::toMat4(glm::quat(transform.Rotation))
			* glm::scale(glm::mat4(1.0f), transform.Scale);

		if (GetParent(scene, entityID) == 0)
			return local;

		glm::mat4 parentTransform = GetWorldTransform(scene, GetParent(scene, entityID));
		return parentTransform * local;
	}

	void CCSceneEntityManager::SetWorldTransform(CCScene* scene, CCUUID entityID, const glm::mat4& worldTransform)
	{
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		if (!entity)
			return;

		auto& transform = entity.GetComponent<Transform>();

		glm::mat4 parentTransform = glm::mat4(1.0f);
		CCUUID parentID = CCSceneEntityManager::GetParent(scene, entityID);
		if (parentID)
			parentTransform = GetWorldTransform(scene, parentID);

		if (glm::determinant(parentTransform) == 0.0f)
			return;

		glm::mat4 localTransform = glm::inverse(parentTransform) * worldTransform;

		glm::vec3 translation, rotation, scale;
		if (Math::DecomposeTransform(localTransform, translation, rotation, scale))
		{
			transform.Translation = translation;
			transform.Rotation = rotation;
			transform.Scale = scale;
		}
	}

	Entity CCSceneEntityManager::GetPrimaryCameraEntity(CCScene* scene)
	{
		auto view = scene->GetRegistry().view<Camera>();
		for (auto entity : view)
		{
			const auto& camera = view.get<Camera>(entity);
			if (camera.Primary)
				return Entity{ entity, scene };
		}
		return {};
	}

	void CCSceneEntityManager::UpdateTransforms(CCScene* scene)
	{
		for (auto entity : scene->GetRegistry().view<Transform>())
		{
			Entity e = { entity, scene };
			if (CCSceneEntityManager::GetParent(scene, e.GetUUID()) == 0)
				UpdateTransformRecursive(scene, e);
		}
	}

	glm::mat4 CCSceneEntityManager::ComposeTransform(CCScene* scene, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
			* glm::toMat4(glm::quat(rotation))
			* glm::scale(glm::mat4(1.0f), scale);

		return transform;
	}

	void CCSceneEntityManager::UpdateTransformRecursive(CCScene* scene, Entity entity)
	{
		auto& transformComponent = entity.GetComponent<Transform>();

		glm::mat4 translation = glm::translate(glm::mat4(1.0f), transformComponent.Translation);
		glm::mat4 rotation =
			glm::rotate(glm::mat4(1.0f), transformComponent.Rotation.z, glm::vec3(0, 0, 1)) *
			glm::rotate(glm::mat4(1.0f), transformComponent.Rotation.y, glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), transformComponent.Rotation.x, glm::vec3(1, 0, 0));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), transformComponent.Scale);

		glm::mat4 localTransform = translation * rotation * scale;

		CCUUID parentUUID = GetParent(scene, entity.GetUUID());
		if (parentUUID != 0)
		{
			auto& parentTransform = GetEntityByUUID(scene, parentUUID).GetComponent<Transform>();
			transformComponent.GlobalTransform = parentTransform.GlobalTransform * localTransform;
		}
		else
		{
			transformComponent.GlobalTransform = localTransform;
		}

		const auto& children = GetChildren(scene, entity.GetUUID());
		for (CCUUID childUUID : children)
		{
			Entity child = GetEntityByUUID(scene, childUUID);
			UpdateTransformRecursive(scene, child);
		}
	}

	template<typename T>
	void CCScene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void CCScene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component) {}

	template<>
	void CCScene::OnComponentAdded<Transform>(Entity entity, Transform& component) {}

	template<>
	void CCScene::OnComponentAdded<Camera>(Entity entity, Camera& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.SceneCamera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void CCScene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}

	template<>
	void CCScene::OnComponentAdded<SpriteRenderer>(Entity entity, SpriteRenderer& component) {}

	template<>
	void CCScene::OnComponentAdded<CircleRenderer>(Entity entity, CircleRenderer& component) {}

	template<>
	void CCScene::OnComponentAdded<MonoScript>(Entity entity, MonoScript& component) {}

	template<>
	void CCScene::OnComponentAdded<NativeScript>(Entity entity, NativeScript& component) {}

	template<>
	void CCScene::OnComponentAdded<Rigidbody2D>(Entity entity, Rigidbody2D& component) {}

	template<>
	void CCScene::OnComponentAdded<BoxCollider2D>(Entity entity, BoxCollider2D& component) {}

	template<>
	void CCScene::OnComponentAdded<CircleCollider2D>(Entity entity, CircleCollider2D& component) {}

	template<>
	void CCScene::OnComponentAdded<Text>(Entity entity, Text& component) {}

	template<>
	void CCScene::OnComponentAdded<AudioSource>(Entity entity, AudioSource& component)
	{
		if (!component.FilePath.empty())
		{
			component.AudioClip = CCAudioEngine::LoadClip(component.FilePath);
			if (component.AudioClip)
				component.AudioClip->SetLooping(component.Loop);
		}
	}
}
