#pragma once

#include "Cataclysm/Core/Buffer.h"
#include <filesystem>

namespace Cataclysm
{
	class FileSystem
	{
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};
}
