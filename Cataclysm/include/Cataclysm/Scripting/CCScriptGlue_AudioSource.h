#pragma once

#include "CCScriptEngine.h"
#include "Cataclysm/Audio/CCAudioEngine.h"
#include "Cataclysm/Asset/CCAssetSystem.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"

namespace Cataclysm
{
	static void AudioSourceComponent_SetAudioClip(CCUUID entityID, CCUUID assetID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& asc = entity.GetComponent<AudioSource>();
		asc.AudioClip = CCAssetSystem::GetAsset<CCAudioClip>(assetID);
	}

	static void AudioSourceComponent_Play(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<AudioSource>(), "Entity does not have AudioSource!");

		auto& asc = entity.GetComponent<AudioSource>();
		CCAudioEngine::Play(asc.AudioClip);
		asc.IsPlaying = true;
	}

	static void AudioSourceComponent_Pause(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<AudioSource>(), "Entity does not have AudioSource!");

		auto& asc = entity.GetComponent<AudioSource>();
		CCAudioEngine::Pause(asc.AudioClip);
		asc.IsPlaying = false;
	}

	static void AudioSourceComponent_Stop(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<AudioSource>(), "Entity does not have AudioSource!");

		auto& asc = entity.GetComponent<AudioSource>();
		CCAudioEngine::Stop(asc.AudioClip);
		asc.IsPlaying = false;
	}
}
