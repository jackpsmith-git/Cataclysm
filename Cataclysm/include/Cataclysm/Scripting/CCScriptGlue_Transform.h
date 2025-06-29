#pragma once

#include "CCScriptEngine.h"
#include "Cataclysm/Math/CCMath.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"

namespace Cataclysm
{
	static void TransformComponent_GetTranslation(CCUUID entityID, glm::vec3* outTranslation)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		*outTranslation = entity.GetComponent<Transform>().Translation;
	}

	static void TransformComponent_SetTranslation(CCUUID entityID, glm::vec3* translation)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<Transform>().Translation = *translation;
	}

	static void TransformComponent_GetRotation(CCUUID entityID, glm::vec3* outRotation)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		*outRotation = entity.GetComponent<Transform>().Rotation;
	}

	static void TransformComponent_SetRotation(CCUUID entityID, glm::vec3* rotation)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<Transform>().Rotation = *rotation;
	}

	static void TransformComponent_GetScale(CCUUID entityID, glm::vec3* outScale)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		*outScale = entity.GetComponent<Transform>().Scale;
	}

	static void TransformComponent_SetScale(CCUUID entityID, glm::vec3* scale)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<Transform>().Scale = *scale;
	}

	static void TransformComponent_GetGlobalTranslation(CCUUID entityID, glm::vec3* outTranslation)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		const glm::mat4& globalTransform = entity.GetComponent<Transform>().GlobalTransform;

		glm::vec3 translation, rotation, scale;
		Math::DecomposeTransform(globalTransform, translation, rotation, scale);

		*outTranslation = translation;
	}

	static void TransformComponent_GetGlobalRotation(CCUUID entityID, glm::vec3* outRotation)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		const glm::mat4& globalTransform = entity.GetComponent<Transform>().GlobalTransform;

		glm::vec3 translation, rotation, scale;
		Math::DecomposeTransform(globalTransform, translation, rotation, scale);

		*outRotation = rotation;
	}

	static void TransformComponent_GetGlobalScale(CCUUID entityID, glm::vec3* outScale)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		const glm::mat4& globalTransform = entity.GetComponent<Transform>().GlobalTransform;

		glm::vec3 translation, rotation, scale;
		Math::DecomposeTransform(globalTransform, translation, rotation, scale);

		*outScale = scale;
	}

	static void TransformComponent_Reset(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<Transform>().Reset();
	}
}
