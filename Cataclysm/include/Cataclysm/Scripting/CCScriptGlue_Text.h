#pragma once

#include "CCScriptEngine.h"
#include "CCScriptUtils.h"
#include "Cataclysm/Asset/CCAssetSystem.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"

namespace Cataclysm
{
	static void TextComponent_SetFont(CCUUID entityID, CCUUID assetID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		auto& tc = entity.GetComponent<Text>();
		tc.FontAsset = CCAssetSystem::GetAsset<Vesuvius::VSFont>(assetID);
		//  SET FONT PATH // tc.FontPath = 
	}

	static MonoString* TextComponent_GetText(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<Text>(), "Entity does not have Text!");
		auto& tc = entity.GetComponent<Text>();
		return CCScriptEngine::CreateString(tc.TextString.c_str());
	}

	static void TextComponent_SetText(CCUUID entityID, MonoString* textString)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<Text>(), "Entity does not have Text!");
		auto& tc = entity.GetComponent<Text>();
		tc.TextString = ScriptUtils::MonoStringToString(textString);
	}

	static void TextComponent_GetColor(CCUUID entityID, glm::vec4* color)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<Text>(), "Entity does not have Text!");
		auto& tc = entity.GetComponent<Text>();
		*color = tc.Color;
	}

	static void TextComponent_SetColor(CCUUID entityID, glm::vec4* color)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<Text>(), "Entity does not have Text!");
		auto& tc = entity.GetComponent<Text>();
		tc.Color = *color;
	}

	static float TextComponent_GetKerning(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<Text>(), "Entity does not have Text!");
		auto& tc = entity.GetComponent<Text>();
		return tc.Kerning;
	}

	static void TextComponent_SetKerning(CCUUID entityID, float kerning)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<Text>(), "Entity does not have Text!");
		auto& tc = entity.GetComponent<Text>();
		tc.Kerning = kerning;
	}

	static float TextComponent_GetLineSpacing(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<Text>(), "Entity does not have Text!");
		auto& tc = entity.GetComponent<Text>();
		return tc.LineSpacing;
	}

	static void TextComponent_SetLineSpacing(CCUUID entityID, float lineSpacing)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");
		CC_CORE_ASSERT(entity.HasComponent<Text>(), "Entity does not have Text!");
		auto& tc = entity.GetComponent<Text>();
		tc.LineSpacing = lineSpacing;
	}

	static void TextComponent_Reset(CCUUID entityID)
	{
		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene does not exist!");
		Entity entity = CCSceneEntityManager::GetEntityByUUID(scene, entityID);
		CC_CORE_ASSERT(entity, "Entity does not exist!");

		entity.GetComponent<Text>().Reset();
	}
}
