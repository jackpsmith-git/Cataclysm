#include "ccpch.h"
#include "Cataclysm/Scripting/CCScriptEngine.h"

#include "Cataclysm/Scripting/CCScriptGlue.h"
#include "Cataclysm/Scripting/CCScriptUtils.h"
#include "Cataclysm/Scripting/CCScriptEngineData.h"
#include "Cataclysm/Core/CCApplication.h"
#include "Cataclysm/Core/CCTimer.h"
#include "Cataclysm/Core/CCBuffer.h"
#include "Cataclysm/Project/CCProject.h"
#include "Cataclysm/Vesuvius/VSTexture.h"

#include <mono/jit/jit.h>
#include <mono/metadata/tabledefs.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-debug.h>

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>  

namespace Cataclysm
{
	static bool s_Initialized = false;
	static CCScriptEngineData* s_Data = nullptr;
	MonoImage* CCScriptEngine::GetCoreAssemblyImage() { return s_Data->CoreAssemblyImage; }
	MonoDomain* CCScriptEngine::GetAppDomain() { return s_Data->AppDomain; }

	static void OnAppAssemblyFileSystemEvent(const std::string& path, const filewatch::Event change_type)
	{
		if (!s_Data->AssemblyReloadPending && change_type == filewatch::Event::modified)
		{
			s_Data->AssemblyReloadPending = true;

			CCApplication::Get().SubmitToMainThread([]()
			{
				s_Data->AppAssemblyFileWatcher.reset();
				CCScriptEngine::ReloadAssembly();
			});
		}
	}

	CCScriptClass::CCScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(isCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	void CCScriptEngine::LoadProjectScripts()
	{
		CC_CORE_ASSERT(s_Initialized && s_Data->RootDomain, "[CCScriptEngine::LoadProjectScripts] Mono must be initialized before loading scripts!");

		if (s_Data->AppDomain)
			UnloadProjectScripts();

		// Create and set new AppDomain
		s_Data->AppDomain = mono_domain_create_appdomain((char*)"CataclysmScriptDomain", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		if (!LoadAssembly("CCML.dll"))
		{
			CC_CORE_ERROR("[CCScriptEngine::LoadProjectScripts] Failed to load core script assembly CCML.dll");
			return;
		}

		auto scriptModulePath = CCProject::GetAssetDirectory() / CCProject::GetActive()->GetConfig().ScriptModulePath;
		if (!LoadAppAssembly(scriptModulePath))
		{
			CC_CORE_ERROR("[CCScriptEngine::LoadProjectScripts] Failed to load project script assembly");
			return;
		}

		LoadAssemblyClasses();
		CCScriptGlue::RegisterComponents();

		s_Data->EntityClass = CCScriptClass("Cataclysm", "Entity", true);
	}

	void CCScriptEngine::UnloadProjectScripts()
	{
		if (!s_Data->AppDomain)
			return;

		// Switch back to root domain before unloading
		mono_domain_set(s_Data->RootDomain, false);
		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
	}

	void CCScriptEngine::Init()
	{
		if (s_Initialized)
			return;

		s_Data = new CCScriptEngineData();

		mono_set_assemblies_path("mono/lib");

		if (s_Data->EnableDebugging)
		{
			PWSTR appDataPathRaw = nullptr;
			SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &appDataPathRaw);
			std::filesystem::path appDataPath = appDataPathRaw;
			CoTaskMemFree(appDataPathRaw);

			std::filesystem::create_directories(appDataPath / "Cataclysm" / "Logs");
			std::string monoLogArg = "--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=" +
				(appDataPath / "Cataclysm" / "Logs" / "MonoDebugger.log").string();

			const char* argv[] = { monoLogArg.c_str(), "--soft-breakpoints" };
			mono_jit_parse_options(2, (char**)argv);
			mono_debug_init(MONO_DEBUG_FORMAT_MONO);
		}

		MonoDomain* rootDomain = mono_jit_init("CataclysmJITRuntime");
		CC_CORE_ASSERT(rootDomain, "[CCScriptEngine::Init] Failed to initialize Mono JIT runtime!");

		s_Data->RootDomain = rootDomain;

		if (s_Data->EnableDebugging)
			mono_debug_domain_create(rootDomain);

		mono_thread_set_main(mono_thread_current());

		CCScriptGlue::RegisterFunctions();

		s_Initialized = true;
	}

	void CCScriptEngine::Shutdown()
	{
		if (!s_Initialized)
			return;

		UnloadProjectScripts(); // Just in case

		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;

		delete s_Data;
		s_Data = nullptr;

		s_Initialized = false;
	}

	void CCScriptEngine::InitGame()
	{
		s_Data = new CCScriptEngineData();

		// Init mono
		{
			mono_set_assemblies_path("mono/lib");

			if (s_Data->EnableDebugging)
			{
				PWSTR appDataPathRaw = nullptr;
				SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &appDataPathRaw);
				std::filesystem::path appDataPath = appDataPathRaw;
				CoTaskMemFree(appDataPathRaw);

				std::filesystem::path monoLogPath = appDataPath / "Cataclysm" / "Logs";
				std::filesystem::create_directories(monoLogPath); // Ensure directory exists

				monoLogPath /= "MonoDebugger.log";
				std::string monoLogArg = "--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=" + monoLogPath.string();

				const char* argv[2] = {
					monoLogArg.c_str(),
					"--soft-breakpoints"
				};

				mono_jit_parse_options(2, (char**)argv);
				mono_debug_init(MONO_DEBUG_FORMAT_MONO);
			}

			MonoDomain* rootDomain = mono_jit_init("CataclysmJITRuntime");
			CC_CORE_ASSERT(rootDomain, "[CCScriptEngine::InitMono] Failed to initialize CataclysmJITRuntime!");

			s_Data->RootDomain = rootDomain;

			if (s_Data->EnableDebugging)
				mono_debug_domain_create(s_Data->RootDomain);

			mono_thread_set_main(mono_thread_current());
		}

		CCScriptGlue::RegisterFunctions();

		bool status = LoadAssembly("../Caterpillar/Resources/Scripts/CCML.dll");
		if (!status)
		{
			CC_CORE_ERROR("[CCScriptEngine::InitGame] Could not load CCML assembly.");
			return;
		}

		auto scriptModulePath = CCProject::GetAssetDirectory() / CCProject::GetActive()->GetConfig().ScriptModulePath;
		status = LoadAppAssembly(scriptModulePath);
		if (!status)
		{
			CC_CORE_ERROR("[CCScriptEngine::Init] Could not load app assembly.");
			return;
		}

		LoadAssemblyClasses();

		CCScriptGlue::RegisterComponents();

		s_Data->EntityClass = CCScriptClass("Cataclysm", "Entity", true);
	}

	bool CCScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppDomain = mono_domain_create_appdomain("CataclysmScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssemblyFilepath = filepath;
		s_Data->CoreAssembly = ScriptUtils::LoadMonoAssembly(filepath, s_Data->EnableDebugging);

		if (s_Data->CoreAssembly == nullptr)
			return false;

		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);

		return true;
	}

	bool CCScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppAssemblyFilepath = filepath;
		s_Data->AppAssembly = ScriptUtils::LoadMonoAssembly(filepath, s_Data->EnableDebugging);
		if (s_Data->AppAssembly == nullptr)
			return false;
		s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);

		s_Data->AppAssemblyFileWatcher = CreateScope<filewatch::FileWatch<std::string>>(filepath.string(), OnAppAssemblyFileSystemEvent);
		s_Data->AssemblyReloadPending = false;

		return true;
	}

	void CCScriptEngine::LoadAssemblyClasses()
	{
		s_Data->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Cataclysm", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, className);
			else
				fullName = className;

			MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, className);

			if (monoClass == entityClass)
				continue;

			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (!isEntity)
				continue;

			CCRef<CCScriptClass> scriptClass = CreateRef<CCScriptClass>(nameSpace, className);
			s_Data->EntityClasses[fullName] = scriptClass;

			// This routine is an iterator routine for retrieving the fields in a class.
			// You must pass a gpointer that points to zero and is treated as an opaque handle
			// to iterate over all of the elements. When no more values are available, the return value is NULL.

			int fieldCount = mono_class_num_fields(monoClass);
			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator))
			{
				const char* fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & FIELD_ATTRIBUTE_PUBLIC)
				{
					MonoType* type = mono_field_get_type(field);
					CCScriptFieldType fieldType = ScriptUtils::MonoTypeToScriptFieldType(type);

					scriptClass->m_Fields[fieldName] = { fieldType, fieldName, field };
				}
			}

		}

		auto& entityClasses = s_Data->EntityClasses;
	}

	void CCScriptEngine::ReloadAssembly()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_Data->AppDomain);

		LoadAssembly(s_Data->CoreAssemblyFilepath);
		LoadAppAssembly(s_Data->AppAssemblyFilepath);
		LoadAssemblyClasses();

		CCScriptGlue::RegisterComponents();

		s_Data->EntityClass = CCScriptClass("Cataclysm", "Entity", true);
	}

	void CCScriptEngine::OnRuntimeStart(CCScene* scene)
	{
		s_Data->SceneContext = scene;
	}

	void CCScriptEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;
		s_Data->EntityInstances.clear();
	}

	void CCScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& sc = entity.GetComponent<MonoScript>();
		if (CCScriptEngine::EntityClassExists(sc.ClassName))
		{
			CCUUID entityID = entity.GetUUID();

			CCRef<CCScriptInstance> instance = CreateRef<CCScriptInstance>(s_Data->EntityClasses[sc.ClassName], entity, s_Data->EntityClass);

			s_Data->EntityInstances[entityID] = instance;

			if (s_Data->EntityScriptFields.find(entityID) != s_Data->EntityScriptFields.end())
			{
				const CCScriptFieldMap& fieldMap = s_Data->EntityScriptFields.at(entityID);
				for (const auto& [name, fieldInstance] : fieldMap)
				{
					if (fieldInstance.Field.Type == CCScriptFieldType::String)
					{
						std::string str = fieldInstance.GetValue<std::string>(); // stored std::string
						MonoString* monoStr = CCScriptEngine::StringToMonoString(str);
						instance->SetFieldValueInternal(name, &monoStr);
					}
					else
					{
						instance->SetFieldValueInternal(name, fieldInstance.m_Buffer);
					}
				}
			}

			instance->InvokeOnCreate();
		}
	}

	void CCScriptEngine::OnUpdateEntity(Entity entity, CCTimestep ts)
	{
		CCUUID entityUUID = entity.GetUUID();
		
		if (s_Data->EntityInstances.find(entityUUID) != s_Data->EntityInstances.end())
		{
			CCRef<CCScriptInstance> instance = s_Data->EntityInstances[entityUUID];
			instance->InvokeOnUpdate((float)ts);
		}
		else
		{
			CCScene* scene = GetSceneContext();
			scene->RuntimeErrorHit("[CCScriptEngine::OnUpdateEntity] Could not find ScriptInstance for entity '" + entity.GetName() + "'");
		}
	}

	void CCScriptEngine::OnCollisionEnter2D(Entity entity, Entity other)
	{
		bool isTrigger = false;
		if (entity.HasComponent<BoxCollider2D>())
		{
			isTrigger = entity.GetComponent<BoxCollider2D>().IsTrigger;
		}
		else if (entity.HasComponent<CircleCollider2D>())
		{
			isTrigger = entity.GetComponent<CircleCollider2D>().IsTrigger;
		}

		bool otherIsTrigger = false;
		if (other.HasComponent<BoxCollider2D>())
		{
			otherIsTrigger = other.GetComponent<BoxCollider2D>().IsTrigger;
		}
		else if (entity.HasComponent<CircleCollider2D>())
		{
			otherIsTrigger = other.GetComponent<CircleCollider2D>().IsTrigger;
		}

		auto uuid = entity.GetUUID();
		MonoObject* instance = GetEntityScriptInstance(uuid)->GetManagedObject();

		if (!instance)
			return;

		MonoClass* monoClass = mono_object_get_class(instance);
		MonoMethod* method = nullptr;
		const char* methodName;

		if (!isTrigger && !otherIsTrigger)
		{
			methodName = "OnCollisionEnter2D";
		}
		else if (isTrigger)
		{
			methodName = "OnTriggerEnter2D";
		}
		else
		{
			return;
		}

		method = mono_class_get_method_from_name(monoClass, methodName, 1);

		if (!method)
			return;

		MonoObject* monoOtherEntity = CreateEntityInstance(other);
		void* args[1] = { monoOtherEntity };
		mono_runtime_invoke(method, instance, args, nullptr);
	}

	void CCScriptEngine::OnCollisionExit2D(Entity entity, Entity other)
	{
		bool isTrigger = false;
		if (entity.HasComponent<BoxCollider2D>())
		{
			isTrigger = entity.GetComponent<BoxCollider2D>().IsTrigger;
		}
		else if (entity.HasComponent<CircleCollider2D>())
		{
			isTrigger = entity.GetComponent<CircleCollider2D>().IsTrigger;
		}

		bool otherIsTrigger = false;
		if (other.HasComponent<BoxCollider2D>())
		{
			otherIsTrigger = other.GetComponent<BoxCollider2D>().IsTrigger;
		}
		else if (entity.HasComponent<CircleCollider2D>())
		{
			otherIsTrigger = other.GetComponent<CircleCollider2D>().IsTrigger;
		}

		auto uuid = entity.GetUUID();

		MonoObject* instance = GetEntityScriptInstance(uuid)->GetManagedObject();
		if (!instance)
			return;

		MonoClass* monoClass = mono_object_get_class(instance);
		MonoMethod* method = nullptr;
		const char* methodName;

		if (!isTrigger && !otherIsTrigger)
		{
			methodName = "OnCollisionExit2D";
		}
		else if (isTrigger)
		{
			methodName = "OnTriggerExit2D";
		}
		else
		{
			return;
		}

		method = mono_class_get_method_from_name(monoClass, methodName, 1);

		if (!method)
			return;

		MonoObject* monoOtherEntity = CreateEntityInstance(other);
		void* args[1] = { monoOtherEntity };
		mono_runtime_invoke(method, instance, args, nullptr);

	}

	MonoObject* CCScriptEngine::CreateEntityInstance(Entity entity)
	{
		MonoClass* entityClass = mono_class_from_name(s_Data->AppAssemblyImage, "Cataclysm", "Entity");
		if (!entityClass)
		{
			// std::cerr << "[CCScriptEngine] Failed to find 'Cataclysm.Entity' class" << std::endl;
			return nullptr;
		}

		MonoObject* instance = mono_object_new(s_Data->AppDomain, entityClass);
		if (!instance)
		{
			std::cerr << "[CCScriptEngine] Failed to create MonoObject for 'Entity'" << std::endl;
			return nullptr;
		}

		MonoMethod* ctor = mono_class_get_method_from_name(entityClass, ".ctor", 1);
		if (!ctor)
		{
			std::cerr << "[CCScriptEngine] Failed to find constructor for 'Entity'" << std::endl;
			return nullptr;
		}

		uint64_t id = entity.GetUUID();
		void* args[1] = { &id };
		mono_runtime_invoke(ctor, instance, args, nullptr);

		return instance;
	}

	MonoObject* CCScriptEngine::GetManagedInstance(CCUUID uuid)
	{
		CC_CORE_ASSERT(s_Data->EntityInstances.find(uuid) != s_Data->EntityInstances.end(), "Instance not found!");
		return s_Data->EntityInstances.at(uuid)->GetManagedObject();
	}

	MonoString* CCScriptEngine::CreateString(const char* string)
	{
		return mono_string_new(s_Data->AppDomain, string);
	}

	MonoObject* CCScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	bool CCScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return s_Data->EntityClasses.find(fullClassName) != s_Data->EntityClasses.end();
	}

	CCScene* CCScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	CCRef<CCScriptInstance> CCScriptEngine::GetEntityScriptInstance(CCUUID entityID)
	{
		auto it = s_Data->EntityInstances.find(entityID);
		if (it == s_Data->EntityInstances.end())
			return nullptr;

		return it->second;
	}

	CCRef<CCScriptClass> CCScriptEngine::GetEntityClass(const std::string& name)
	{
		if (s_Data->EntityClasses.find(name) == s_Data->EntityClasses.end())
			return nullptr;

		return s_Data->EntityClasses.at(name);
	}

	std::unordered_map<std::string, CCRef<CCScriptClass>> CCScriptEngine::GetEntityClasses()
	{
		return s_Data->EntityClasses;
	}

	CCScriptFieldMap& CCScriptEngine::GetScriptFieldMap(Entity entity)
	{
		CC_CORE_ASSERT(entity, "[CCScriptEngine::GetScriptFieldMap] Entity does not exist!");

		CCUUID entityID = entity.GetUUID();
		return s_Data->EntityScriptFields[entityID];
	}

	CCUUID CCScriptEngine::GetEntityIDFromComponentField(CCRef<CCScriptInstance> instance, const CCScriptField& field, const std::string& name)
	{
		void* componentObject = instance->GetRawFieldValue(name);
		if (!componentObject)
			return 0;

		MonoClass* componentClass = mono_object_get_class((MonoObject*)componentObject);
		MonoClassField* entityField = mono_class_get_field_from_name(componentClass, "Entity");
		if (!entityField)
			return 0;

		uint64_t entityID = 0;
		mono_field_get_value((MonoObject*)componentObject, entityField, &entityID);
		return entityID;
	}

	void CCScriptEngine::SetComponentFieldWithEntityID(CCRef<CCScriptInstance> instance, const CCScriptField& field, const std::string& name, CCUUID entityID)
	{
		// STEP 1: Create C# Entity object
		MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Cataclysm", "Entity");
		if (!entityClass)
		{
			CC_CORE_ERROR("Entity class not found");
			return;
		}

		MonoMethod* createInternal = mono_class_get_method_from_name(entityClass, "CreateInternal", 1);
		if (!createInternal)
		{
			CC_CORE_ERROR("Entity.CreateInternal method not found");
			return;
		}

		uint64_t rawID = (uint64_t)entityID;
		void* args[1] = { &rawID };

		MonoObject* exc = nullptr;
		MonoObject* entityObject = mono_runtime_invoke(createInternal, nullptr, args, &exc);
		if (exc)
		{
			char* excStr = mono_string_to_utf8(mono_object_to_string(exc, nullptr));
			CC_CORE_ERROR("Exception calling CreateInternal: {0}", excStr);
			mono_free(excStr);
			return;
		}

		if (!entityObject)
		{
			CC_CORE_ERROR("CreateInternal returned null MonoObject");
			return;
		}

		// STEP 2: Construct the actual component (e.g., AudioSource) using the Entity
		MonoClass* scriptClass = mono_object_get_class(instance->GetManagedObject());
		MonoClassField* componentField = mono_class_get_field_from_name(scriptClass, name.c_str());
		MonoType* fieldType = mono_field_get_type(componentField);
		MonoClass* componentClass = mono_type_get_class(fieldType);		if (!componentClass)
		{
			// CC_CORE_ERROR("Component class not found!");
			return;
		}

		MonoMethod* ctor = mono_class_get_method_from_name(componentClass, ".ctor", 1);
		if (!ctor)
		{
			// CC_CORE_ERROR("Constructor not found for component class!");
			return;
		}

		MonoObject* componentObject = mono_object_new(mono_domain_get(), componentClass);
		void* argsComponent[1] = { entityObject };

		mono_runtime_invoke(ctor, componentObject, argsComponent, &exc);
		if (exc)
		{
			char* excStr = mono_string_to_utf8(mono_object_to_string(exc, nullptr));
			// CC_CORE_ERROR("Exception in component constructor!");
			mono_free(excStr);
			return;
		}

		// STEP 3: Assign constructed component instance to field
		instance->SetRawFieldValue(name, componentObject);
	}

	MonoString* CCScriptEngine::StringToMonoString(const std::string& str)
	{
		return mono_string_new(CCScriptEngine::GetAppDomain(), str.c_str());
	}

	std::string CCScriptEngine::MonoStringToString(MonoString* string)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		return str;
	}
}
