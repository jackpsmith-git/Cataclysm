#pragma once

#include "CCScriptExtern.h"
#include "CCScriptField.h"
#include "CCScriptFieldInstance.h"

#include <string>
#include <map>
#include <unordered_map>

namespace Cataclysm
{
	using CCScriptFieldMap = std::unordered_map<std::string, CCScriptFieldInstance>;

	class CCScriptClass
	{
	public:
		CCScriptClass() = default;
		CCScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		const std::map<std::string, CCScriptField>& GetFields() const { return m_Fields; }

		const CCScriptField& GetField(const std::string& name) const
		{
			auto it = m_Fields.find(name);
			CC_CORE_ASSERT(it != m_Fields.end(), "Field not found!");
			return it->second;
		}

		bool HasField(const std::string& name) const
		{
			return m_Fields.find(name) != m_Fields.end();
		}

	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		std::map<std::string, CCScriptField> m_Fields;

		MonoClass* m_MonoClass = nullptr;

		friend class CCScriptEngine;
	};
}
