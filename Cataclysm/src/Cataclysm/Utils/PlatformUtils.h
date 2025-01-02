#pragma once

#include <string>

namespace Cataclysm
{
	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter, const char* extension);
		static std::string SaveFile(const char* filter, const char* extension);
	};

	class Time
	{
	public:
		static float GetTime();
	};
}