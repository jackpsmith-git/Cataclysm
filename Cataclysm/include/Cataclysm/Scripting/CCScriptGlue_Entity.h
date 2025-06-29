#pragma once

#include "CCScriptEngine.h"
#include "CCScriptUtils.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"
#include "Cataclysm.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

namespace Cataclysm
{
	static void Entity_InstantiateEmptyWithoutName(CCUUID* outEntityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::CreateEntity(scene, "NewEntity");
		*outEntityID = entity.GetUUID();
	}

	static void Entity_InstantiateEmpty(MonoString* name, CCUUID* outEntityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::CreateEntity(scene, ScriptUtils::MonoStringToString(name));
		*outEntityID = entity.GetUUID();
	}

	static void Entity_InstantiateEmptyAsChild(MonoString* name, CCUUID parentID, CCUUID* outEntityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::CreateEntity(scene, ScriptUtils::MonoStringToString(name));
		CCSceneEntityManager::SetParent(scene, entity.GetUUID(), parentID);
		*outEntityID = entity.GetUUID();
	}

	static void Entity_Destroy(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		CCSceneEntityManager::DestroyEntity(scene, entity);
	}

	static void Entity_AddBoxCollider2DComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<BoxCollider2D>())
			entity.AddComponent<BoxCollider2D>();
	}

	static void Entity_AddCameraComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<Camera>())
			entity.AddComponent<Camera>();
	}

	static void Entity_AddAudioSourceComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<AudioSource>())
			entity.AddComponent<AudioSource>();
	}

	static void Entity_AddCircleCollider2DComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<CircleCollider2D>())
			entity.AddComponent<CircleCollider2D>();
	}

	static void Entity_AddCircleRendererComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<CircleRenderer>())
			entity.AddComponent<CircleRenderer>();
	}

	static void Entity_AddRigidbody2DComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<Rigidbody2D>())
			entity.AddComponent<Rigidbody2D>();
	}

	static void Entity_AddSpriteRendererComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<SpriteRenderer>())
			entity.AddComponent<SpriteRenderer>();
	}

	static void Entity_AddTextComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (!entity.HasComponent<Text>())
			entity.AddComponent<Text>();
	}

	static void Entity_RemoveAudioSourceComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<AudioSource>())
			entity.RemoveComponent<AudioSource>();
	}

	static void Entity_RemoveBoxCollider2DComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<BoxCollider2D>())
			entity.RemoveComponent<BoxCollider2D>();
	}

	static void Entity_RemoveCameraComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<Camera>())
			entity.RemoveComponent<Camera>();
	}

	static void Entity_RemoveCircleCollider2DComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<CircleCollider2D>())
			entity.RemoveComponent<CircleCollider2D>();
	}

	static void Entity_RemoveCircleRendererComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<CircleRenderer>())
			entity.RemoveComponent<CircleRenderer>();
	}

	static void Entity_RemoveSpriteRendererComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<SpriteRenderer>())
			entity.RemoveComponent<SpriteRenderer>();
	}

	static void Entity_RemoveTextComponent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");

		if (entity.HasComponent<Text>())
			entity.RemoveComponent<Text>();
	}

	static uint64_t Entity_FindEntityByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);

		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::FindEntityByName(scene, nameCStr);
		mono_free(nameCStr);

		if (!entity)
			return 0;

		return entity.GetUUID();
	}

	static bool Entity_GetEnabled(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity not found!");
		return entity.GetComponent<IDComponent>().Enabled;
	}

	static void Entity_SetEnabled(CCUUID entityID, bool enabled)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		entity.GetComponent<IDComponent>().Enabled = enabled;
	}

	static void Entity_SetParent(CCUUID entityID, CCUUID parentID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		Entity parent = CCSceneEntityManager::GetEntityByUUID(scene, parentID);
		CC_CORE_ASSERT(entity, "Parent does not exist!");

		CCSceneEntityManager::SetParent(scene, entityID, parentID);
	}

	static void Entity_RemoveParent(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		CCSceneEntityManager::RemoveParent(scene, entityID);
	}

	static void Entity_GetParent(CCUUID entityID, CCUUID* outParentID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		*outParentID = CCSceneEntityManager::GetParent(scene, entityID);
	}
}
