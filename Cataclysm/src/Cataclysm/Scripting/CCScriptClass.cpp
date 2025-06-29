#include "ccpch.h"
#include "Cataclysm/Scripting/CCScriptClass.h"

#include "Cataclysm/Scripting/CCScriptEngine.h"

#include <mono/metadata/class.h>
#include <mono/metadata/object.h>

namespace Cataclysm
{
	MonoObject* CCScriptClass::Instantiate() { return CCScriptEngine::InstantiateClass(m_MonoClass); }
	MonoMethod* CCScriptClass::GetMethod(const std::string& name, int parameterCount) { return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount); }

	MonoObject* CCScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		MonoObject* exception = nullptr;
		return mono_runtime_invoke(method, instance, params, &exception);
	}
}
