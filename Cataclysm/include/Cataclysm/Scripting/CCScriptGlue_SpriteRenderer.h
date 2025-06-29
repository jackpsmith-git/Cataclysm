#pragma once

#include "CCScriptEngine.h"
#include "Cataclysm/Asset/CCAssetSystem.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"

namespace Cataclysm
{
	static void SpriteRendererComponent_SetTexture(CCUUID entityID, CCUUID assetID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& src = entity.GetComponent<SpriteRenderer>();
		src.Texture = CCAssetSystem::GetAsset<Vesuvius::VSTexture2D>(assetID);
	}

	static void SpriteRendererComponent_GetColor(CCUUID entityID, glm::vec4* outColor)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		*outColor = entity.GetComponent<SpriteRenderer>().Color;
	}

	static void SpriteRendererComponent_SetColor(CCUUID entityID, glm::vec4* color)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<SpriteRenderer>().Color = *color;
	}

	static float SpriteRendererComponent_GetTilingFactor(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<SpriteRenderer>(), "Entity does not have SpriteRenderer!");
		auto& src = entity.GetComponent<SpriteRenderer>();
		return src.TilingFactor;
	}

	static void SpriteRendererComponent_SetTilingFactor(CCUUID entityID, float tilingFactor)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<SpriteRenderer>(), "Entity does not have SpriteRenderer!");
		auto& src = entity.GetComponent<SpriteRenderer>();
		src.TilingFactor = tilingFactor;
	}

	static void SpriteRendererComponent_Reset(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<SpriteRenderer>().Reset();
	}
}
