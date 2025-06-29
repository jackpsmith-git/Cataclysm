#pragma once

#include "Cataclysm/ECS/CCEntity.h"
#include "CCScriptClass.h"

namespace Cataclysm
{
	class CCScriptInstance
	{
	public:
		CCScriptInstance(CCRef<CCScriptClass> scriptClass, Entity entity, CCScriptClass entityClass);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);

		CCRef<CCScriptClass> GetScriptClass() { return m_ScriptClass; }

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			static_assert(sizeof(T) <= 64, "Type too large!");

			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();

			return *(T*)s_FieldValueBuffer;
		}

		template<>
		std::string CCScriptInstance::GetFieldValue<std::string>(const std::string& name)
		{
			return GetStringValue(name);
		}

		template<typename T>
		void SetFieldValue(const std::string& name, T value)
		{
			static_assert(sizeof(T) <= 64, "Type too large!");

			SetFieldValueInternal(name, &value);
		}

		template<>
		inline void CCScriptInstance::SetFieldValue<std::string>(const std::string& name, std::string value)
		{
			std::string* str = new std::string(std::move(value));
			memcpy(s_FieldValueBuffer, &str, sizeof(std::string*));
			SetFieldValueInternal(name, s_FieldValueBuffer);
		}

		MonoObject* GetRawFieldValue(const std::string& name);
		void SetRawFieldValue(const std::string& name, MonoObject* value);

		MonoObject* GetManagedObject() { return m_Instance; }
	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);

		std::string GetStringValue(const std::string& name);

		static MonoMethod* GetCreateMethod(CCScriptFieldType type);

	private:
		CCRef<CCScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;

		inline static char s_FieldValueBuffer[64];

		friend class CCScriptEngine;
		friend struct CCScriptFieldInstance;
	};
}
