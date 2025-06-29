#pragma once

#include "CCScriptEngine.h"
#include "CCScriptUtils.h"
#include "Cataclysm.h"

namespace Cataclysm
{
	static void Debug_Log(MonoString* message)
	{
		std::string str = ScriptUtils::MonoStringToString(message);
		CC_CORE_TRACE(str);
	}

	static void Debug_Info(MonoString* message)
	{
		std::string str = ScriptUtils::MonoStringToString(message);
		CC_CORE_INFO(str);
	}

	static void Debug_Warn(MonoString* message)
	{
		std::string str = ScriptUtils::MonoStringToString(message);
		CC_CORE_WARN(str);
	}

	static void Debug_Error(MonoString* message)
	{
		std::string str = ScriptUtils::MonoStringToString(message);

		CCScene* scene = CCScriptEngine::GetSceneContext();
		CC_CORE_ASSERT(scene, "Scene not found!");

		scene->RuntimeErrorHit(str);
	}
}
