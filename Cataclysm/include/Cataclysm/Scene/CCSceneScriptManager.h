#pragma once

#include "Cataclysm/Scene/CCScene.h"

namespace Cataclysm
{
	class CCSceneScriptManager
	{
	public:
		static void OnScriptingStart(CCScene* scene);
		static void OnScriptingStop(CCScene* scene);
		static void ProcessScripts(CCScene* scene, float ts);
	};
}
