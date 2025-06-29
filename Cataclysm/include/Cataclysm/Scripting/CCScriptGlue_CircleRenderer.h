#pragma once

#include "CCScriptEngine.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"

namespace Cataclysm
{
	static void CircleRendererComponent_GetColor(CCUUID entityID, glm::vec4* outColor)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		*outColor = entity.GetComponent<CircleRenderer>().Color;
	}

	static void CircleRendererComponent_SetColor(CCUUID entityID, glm::vec4* color)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<CircleRenderer>().Color = *color;
	}

	static float CircleRendererComponent_GetThickness(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleRenderer>(), "Entity does not have CircleRenderer!");
		auto& crc = entity.GetComponent<CircleRenderer>();
		return crc.Thickness;
	}

	static void CircleRendererComponent_SetThickness(CCUUID entityID, float thickness)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleRenderer>(), "Entity does not have CircleRenderer!");
		auto& crc = entity.GetComponent<CircleRenderer>();
		crc.Thickness = thickness;
	}

	static float CircleRendererComponent_GetFade(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleRenderer>(), "Entity does not have CircleRenderer!");
		auto& crc = entity.GetComponent<CircleRenderer>();
		return crc.Fade;
	}

	static void CircleRendererComponent_SetFade(CCUUID entityID, float fade)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleRenderer>(), "Entity does not have CircleRenderer!");
		auto& crc = entity.GetComponent<CircleRenderer>();
		crc.Fade = fade;
	}

	static void CircleRendererComponent_Reset(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<CircleRenderer>().Reset();
	}
}
