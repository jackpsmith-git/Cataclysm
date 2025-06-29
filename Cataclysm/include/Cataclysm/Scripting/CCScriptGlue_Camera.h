#pragma once

#include "CCScriptEngine.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"

namespace Cataclysm
{
	static bool CameraComponent_GetPrimary(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		return cc.Primary;
	}

	static void CameraComponent_SetPrimary(CCUUID entityID, bool primary)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		cc.Primary = primary;
	}

	static bool CameraComponent_GetFixedAspectRatio(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		return cc.FixedAspectRatio;
	}

	static void CameraComponent_SetFixedAspectRatio(CCUUID entityID, bool fixedAspectRatio)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		cc.FixedAspectRatio = fixedAspectRatio;
	}

	static float CameraComponent_GetOrthographicSize(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		return cc.SceneCamera.GetOrthographicSize();
	}

	static void CameraComponent_SetOrthographicSize(CCUUID entityID, float size)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		cc.SceneCamera.SetOrthographicSize(size);
	}

	static float CameraComponent_GetPerspectiveVerticalFOV(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		return cc.SceneCamera.GetPerspectiveVerticalFOV();
	}

	static void CameraComponent_SetPerspectiveVerticalFOV(CCUUID entityID, float fov)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		cc.SceneCamera.SetPerspectiveVerticalFOV(fov);
	}

	static float CameraComponent_GetOrthographicNearClip(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		return cc.SceneCamera.GetOrthographicNearClip();
	}

	static void CameraComponent_SetOrthographicNearClip(CCUUID entityID, float clip)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		cc.SceneCamera.SetOrthographicNearClip(clip);
	}

	static float CameraComponent_GetOrthographicFarClip(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		return cc.SceneCamera.GetOrthographicFarClip();
	}

	static void CameraComponent_SetOrthographicFarClip(CCUUID entityID, float clip)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		cc.SceneCamera.SetOrthographicFarClip(clip);
	}

	static float CameraComponent_GetPerspectiveNearClip(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		return cc.SceneCamera.GetPerspectiveNearClip();
	}

	static void CameraComponent_SetPerspectiveNearClip(CCUUID entityID, float clip)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		cc.SceneCamera.SetPerspectiveNearClip(clip);
	}

	static float CameraComponent_GetPerspectiveFarClip(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		return cc.SceneCamera.GetPerspectiveFarClip();
	}

	static void CameraComponent_SetPerspectiveFarClip(CCUUID entityID, float clip)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& cc = entity.GetComponent<Camera>();
		cc.SceneCamera.SetPerspectiveFarClip(clip);
	}

	static void CameraComponent_Reset(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<Camera>().Reset();
	}
}
