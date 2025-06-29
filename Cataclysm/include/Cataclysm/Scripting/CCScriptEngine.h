#pragma once

#include "CCScriptInstance.h"

#include "Cataclysm/Scene/CCScene.h"
#include "Cataclysm/ECS/CCEntity.h"

namespace Cataclysm
{
	class CCScriptEngine
	{
	public:
		static void Init();
		static void InitGame();
		static void Shutdown();
		static void LoadProjectScripts();
		static void UnloadProjectScripts();

		static bool LoadAssembly(const std::filesystem::path& filepath);
		static bool LoadAppAssembly(const std::filesystem::path& filepath);
		static void ReloadAssembly();
		static void OnRuntimeStart(CCScene* scene);
		static void OnRuntimeStop();

		static bool EntityClassExists(const std::string& fullClassName);
		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, CCTimestep ts);
		static void OnCollisionEnter2D(Entity entity, Entity other);
		static void OnCollisionExit2D(Entity entity, Entity other);

		static CCScene* GetSceneContext();
		static CCRef<CCScriptInstance> GetEntityScriptInstance(CCUUID entityID);
		static CCRef<CCScriptClass> GetEntityClass(const std::string& name);
		static std::unordered_map<std::string, CCRef<CCScriptClass>> GetEntityClasses();
		static CCScriptFieldMap& GetScriptFieldMap(Entity entity);
		static MonoImage* GetCoreAssemblyImage();
		static MonoObject* GetManagedInstance(CCUUID uuid);
		static MonoDomain* GetAppDomain();

		static MonoString* CreateString(const char* string);
		static MonoObject* CCScriptEngine::CreateEntityInstance(Entity entity);

		static CCUUID GetEntityIDFromComponentField(CCRef<CCScriptInstance> instance, const CCScriptField& field, const std::string& name);
		static void SetComponentFieldWithEntityID(CCRef<CCScriptInstance> instance, const CCScriptField& field, const std::string& name, CCUUID entityID);
		static MonoString* StringToMonoString(const std::string& str);
		static std::string MonoStringToString(MonoString* string);
	private:

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();

		friend class CCScriptClass;
		friend class CCScriptGlue;
	};
}
