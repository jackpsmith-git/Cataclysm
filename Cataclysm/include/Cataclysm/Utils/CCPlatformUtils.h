#pragma once

#include <string>

namespace Cataclysm
{
	class CCFileDialogs
	{
	public:
		static std::string OpenFile(const char* filter, const char* extension);
		static std::string SaveFile(const char* filter, const char* extension);
	};

	class CCTime
	{
	public:
		static float GetTime();
	};
}
