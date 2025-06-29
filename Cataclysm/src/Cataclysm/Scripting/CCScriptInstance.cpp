#include "ccpch.h"
#include "Cataclysm/Scripting/CCScriptInstance.h"

#include "Cataclysm/Scripting/CCScriptUtils.h"

#include <mono/metadata/object.h>
#include <mono/metadata/appdomain.h>

namespace Cataclysm
{
	static std::unordered_map<CCScriptFieldType, MonoMethod*> s_CreateMethods = {
		{ CCScriptFieldType::Entity, nullptr },
		{ CCScriptFieldType::Texture2D, nullptr },
		{ CCScriptFieldType::AudioClip, nullptr },
		{ CCScriptFieldType::Font, nullptr }
	};

	static std::unordered_map<CCScriptFieldType, const char*> s_CreateMethodNames = {
		{ CCScriptFieldType::Entity, "Cataclysm.Entity:CreateInternal(ulong)" },
		{ CCScriptFieldType::Texture2D, "Cataclysm.Texture2D:CreateInternal(ulong)" },
		{ CCScriptFieldType::AudioClip, "Cataclysm.AudioClip:CreateInternal(ulong)" },
		{ CCScriptFieldType::Font, "Cataclysm.Font:CreateInternal(ulong)" }
	};

	CCScriptInstance::CCScriptInstance(CCRef<CCScriptClass> scriptClass, Entity entity, CCScriptClass entityClass)
		: m_ScriptClass(scriptClass)
	{
		m_Instance = scriptClass->Instantiate();

		m_Constructor = entityClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = scriptClass->GetMethod("OnCreate", 0);
		m_OnUpdateMethod = scriptClass->GetMethod("OnUpdate", 1);

		{
			CCUUID entityID = entity.GetUUID();
			void* param = &entityID;
			m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &param);
		}
	}

	MonoMethod* CCScriptInstance::GetCreateMethod(CCScriptFieldType type)
	{
		MonoMethod* createMethod = s_CreateMethods[type];

		if (createMethod)
			return createMethod;

		MonoMethodDesc* desc = mono_method_desc_new(s_CreateMethodNames[type], true);
		createMethod = mono_method_desc_search_in_image(desc, CCScriptEngine::GetCoreAssemblyImage());
		mono_method_desc_free(desc);

		if (!createMethod)
		{
			CC_CORE_ERROR("[CCScriptInstance::GetCreateMethod] Script field type creation method not found!");
			return nullptr;
		}

		return createMethod;
	}

	void CCScriptInstance::InvokeOnCreate()
	{
		if (m_OnCreateMethod)
			m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
	}

	void CCScriptInstance::InvokeOnUpdate(float ts)
	{
		if (m_OnUpdateMethod)
		{
			void* param = &ts;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &param);
		}
	}

	bool CCScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		const auto& fields = m_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const CCScriptField& field = it->second;

		// Reference types such as Entities and Asset types
		if (ScriptUtils::IsReferenceType(field.Type))
		{
			MonoObject* object = nullptr;
			mono_field_get_value(m_Instance, field.ClassField, &object);

			if (object)
			{
				MonoClass* monoClass = mono_object_get_class(object);
				MonoProperty* idProp = mono_class_get_property_from_name(monoClass, "ID");
				if (idProp)
				{
					MonoObject* result = mono_property_get_value(idProp, object, nullptr, nullptr);
					if (result)
					{
						*(uint64_t*)buffer = *(uint64_t*)mono_object_unbox(result);
						return true;
					}
				}
			}

			*(uint64_t*)buffer = 0;
			return true;
		}
		else if (field.Type == CCScriptFieldType::String)
		{
			MonoString* monoStr = (MonoString*)mono_field_get_value_object(CCScriptEngine::GetAppDomain(), field.ClassField, m_Instance);
			memcpy(buffer, &monoStr, sizeof(MonoString*));
			return true;
		}

		// Value types such as int, float, etc.
		mono_field_get_value(m_Instance, field.ClassField, buffer);
		return true;
	}

	bool CCScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const CCScriptField& field = it->second;

		// Reference types such as Entities and Asset types
		if (ScriptUtils::IsReferenceType(field.Type))
		{
			uint64_t id = *(const uint64_t*)value;
			MonoMethod* createMethod = GetCreateMethod(field.Type);

			if (!createMethod)
			{
				CC_CORE_ERROR("Failed to find create method for field type!");
				return false;
			}

			void* args[1] = { &id };
			MonoObject* object = mono_runtime_invoke(createMethod, nullptr, args, nullptr);

			if (!object)
			{
				CC_CORE_ERROR("mono_runtime_invoke returned null for create method!");
				return false;
			}

			mono_field_set_value(m_Instance, field.ClassField, object);
			return true;
		}
		else if (field.Type == CCScriptFieldType::String)
		{
			MonoString* monoStr = *(MonoString* const*)value;
			mono_field_set_value(m_Instance, field.ClassField, monoStr);
			return true;
		}

		// Value types such as int, float, etc.
		mono_field_set_value(m_Instance, field.ClassField, (void*)value);
		return true;
	}

	std::string CCScriptInstance::GetStringValue(const std::string& name)
	{
		bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
		if (!success)
			return std::string();

		MonoString* monoStr = *(MonoString**)s_FieldValueBuffer;
		if (!monoStr)
			return std::string();

		char* utf8Str = mono_string_to_utf8(monoStr);
		std::string result = utf8Str ? std::string(utf8Str) : std::string();
		mono_free(utf8Str);

		return result;
	}

	MonoObject* CCScriptInstance::GetRawFieldValue(const std::string& name)
	{
		if (!m_Instance)
			return nullptr;

		MonoClass* klass = mono_object_get_class(m_Instance);
		MonoClassField* field = mono_class_get_field_from_name(klass, name.c_str());
		if (!field)
			return nullptr;

		MonoObject* value = mono_field_get_value_object(mono_domain_get(), field, m_Instance);
		return value;
	}

	void CCScriptInstance::SetRawFieldValue(const std::string& name, MonoObject* value)
	{
		if (!m_Instance)
			return;

		MonoClass* klass = mono_object_get_class(m_Instance);
		MonoClassField* field = mono_class_get_field_from_name(klass, name.c_str());
		if (!field)
			return;

		mono_field_set_value(m_Instance, field, value);
	}
}
