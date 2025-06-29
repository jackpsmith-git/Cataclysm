#pragma once

#include "CCScriptEngine.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"

namespace Cataclysm
{
	static void BoxCollider2DComponent_GetOffset(CCUUID entityID, glm::vec2* offset)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		*offset = bc2d.Offset;
	}

	static void BoxCollider2DComponent_SetOffset(CCUUID entityID, glm::vec2* offset)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		bc2d.Offset = *offset;
	}

	static void BoxCollider2DComponent_GetSize(CCUUID entityID, glm::vec2* size)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		*size = bc2d.Size;
	}

	static void BoxCollider2DComponent_SetSize(CCUUID entityID, glm::vec2* size)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		bc2d.Size = *size;
	}

	static float BoxCollider2DComponent_GetDensity(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		return bc2d.Density;
	}

	static void BoxCollider2DComponent_SetDensity(CCUUID entityID, float density)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		bc2d.Density = density;
	}

	static float BoxCollider2DComponent_GetFriction(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		return bc2d.Friction;
	}

	static void BoxCollider2DComponent_SetFriction(CCUUID entityID, float friction)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		bc2d.Friction = friction;
	}

	static float BoxCollider2DComponent_GetRestitution(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		return bc2d.Restitution;
	}

	static void BoxCollider2DComponent_SetRestitution(CCUUID entityID, float restitution)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		bc2d.Restitution = restitution;
	}

	static float BoxCollider2DComponent_GetRestitutionThreshold(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		return bc2d.RestitutionThreshold;
	}

	static void BoxCollider2DComponent_SetRestitutionThreshold(CCUUID entityID, float restitutionThreshold)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		bc2d.RestitutionThreshold = restitutionThreshold;
	}

	static bool BoxCollider2DComponent_GetIsTrigger(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		return bc2d.IsTrigger;
	}

	static void BoxCollider2DComponent_SetIsTrigger(CCUUID entityID, bool isTrigger)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<BoxCollider2D>(), "Entity does not have BoxCollider2D!");
		auto& bc2d = entity.GetComponent<BoxCollider2D>();
		bc2d.IsTrigger = isTrigger;
	}

	static void BoxCollider2DComponent_Reset(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<BoxCollider2D>().Reset();
	}

}
