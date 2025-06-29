#include "ccpch.h"
#include "Cataclysm/Scene/CCSceneScriptManager.h"

#include "Cataclysm/Scene/CCSceneEntityManager.h"
#include "Cataclysm/Scripting/CCScriptEngine.h"
#include "Cataclysm/ECS/CCScriptableEntity.h"

namespace Cataclysm
{
	void CCSceneScriptManager::OnScriptingStart(CCScene* scene)
	{
		CCScriptEngine::OnRuntimeStart(scene);

		auto view = scene->GetRegistry().view<MonoScript>();
		for (auto e : view)
		{
			Entity entity = { e, scene };
			CCScriptEngine::OnCreateEntity(entity);
		}
	}

	void CCSceneScriptManager::OnScriptingStop(CCScene* scene)
	{
		CCScriptEngine::OnRuntimeStop();
	}

	void CCSceneScriptManager::ProcessScripts(CCScene* scene, float ts)
	{
		auto view = scene->GetRegistry().view<MonoScript>();
		for (auto e : view)
		{
			Entity entity = { e, scene };
			if (CCSceneEntityManager::IsEnabled(scene, entity))
			{
				CCScriptEngine::OnUpdateEntity(entity, ts);
			}
		}
	}
}
