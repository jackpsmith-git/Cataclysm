#pragma once

#include "filewatch.h"

namespace Cataclysm
{
	struct CCScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		std::filesystem::path CoreAssemblyFilepath;
		std::filesystem::path AppAssemblyFilepath;

		CCScriptClass EntityClass;

		std::unordered_map<std::string, CCRef<CCScriptClass>> EntityClasses;
		std::unordered_map<CCUUID, CCRef<CCScriptInstance>> EntityInstances;
		std::unordered_map<CCUUID, CCScriptFieldMap> EntityScriptFields;

		CCScope<filewatch::FileWatch<std::string>> AppAssemblyFileWatcher;
		bool AssemblyReloadPending = false;

#ifdef CC_DEBUG
		bool EnableDebugging = true;
#else
		bool EnableDebugging = false;
#endif

		CCScene* SceneContext = nullptr;
	};
}
