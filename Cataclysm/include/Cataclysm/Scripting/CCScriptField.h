#pragma once

#include "CCScriptFieldType.h"
#include "CCScriptExtern.h"
#include <string>

namespace Cataclysm
{
	struct CCScriptField
	{
		CCScriptFieldType Type;
		std::string Name;

		MonoClassField* ClassField;
	};
}
