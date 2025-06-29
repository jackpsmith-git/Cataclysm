#pragma once

#include "CCScriptEngine.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"

namespace Cataclysm
{
	static void CircleCollider2DComponent_GetOffset(CCUUID entityID, glm::vec2* offset)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		*offset = cc2d.Offset;
	}

	static void CircleCollider2DComponent_SetOffset(CCUUID entityID, glm::vec2* offset)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		cc2d.Offset = *offset;
	}

	static float CircleCollider2DComponent_GetRadius(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		return cc2d.Radius;
	}

	static void CircleCollider2DComponent_SetRadius(CCUUID entityID, float radius)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		cc2d.Radius = radius;
	}

	static float CircleCollider2DComponent_GetDensity(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		return cc2d.Density;
	}

	static void CircleCollider2DComponent_SetDensity(CCUUID entityID, float density)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		cc2d.Density = density;
	}

	static float CircleCollider2DComponent_GetFriction(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		return cc2d.Friction;
	}

	static void CircleCollider2DComponent_SetFriction(CCUUID entityID, float friction)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		cc2d.Friction = friction;
	}

	static float CircleCollider2DComponent_GetRestitution(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		return cc2d.Restitution;
	}

	static void CircleCollider2DComponent_SetRestitution(CCUUID entityID, float restitution)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		cc2d.Restitution = restitution;
	}

	static float CircleCollider2DComponent_GetRestitutionThreshold(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		return cc2d.RestitutionThreshold;
	}

	static void CircleCollider2DComponent_SetRestitutionThreshold(CCUUID entityID, float restitutionThreshold)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		cc2d.RestitutionThreshold = restitutionThreshold;
	}

	static bool CircleCollider2DComponent_GetIsTrigger(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		return cc2d.IsTrigger;
	}

	static void CircleCollider2DComponent_SetIsTrigger(CCUUID entityID, bool isTrigger)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<CircleCollider2D>(), "Entity does not have CircleCollider2D!");
		auto& cc2d = entity.GetComponent<CircleCollider2D>();
		cc2d.IsTrigger = isTrigger;
	}

	static void CircleCollider2DComponent_Reset(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<CircleCollider2D>().Reset();
	}
}
