#pragma once

#include "Cataclysm/Core/Buffer.h"

#include <filesystem>

namespace Cataclysm
{
	class File
	{
	public:
		static Buffer Read(const std::filesystem::path& filepath);
	};
}
