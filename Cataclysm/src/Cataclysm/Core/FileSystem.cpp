#include "ccpch.h"
#include "FileSystem.h"

namespace Cataclysm
{
	Buffer FileSystem::ReadFileBinary(const std::filesystem::path& filepath)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			return {};
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint64_t size = end - stream.tellg();

		if (size == 0)
		{
			return {};
		}

		Buffer buffer(size);
		stream.read(buffer.As<char>(), size);
		stream.close();
		return buffer;
	}
}
