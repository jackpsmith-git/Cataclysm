#pragma once

#include "Cataclysm/Core/CCBuffer.h"

#include <filesystem>

namespace Cataclysm
{
	class CCFile
	{
	public:
		static CCBuffer Read(const std::filesystem::path& filepath);
	};
}
