#pragma once

namespace Cataclysm
{
	struct CCWindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		CCWindowProps(const std::string& title = "Cataclysm Engine",
			uint32_t width = 1600,
			uint32_t height = 900)
			: Title(title), Width(width), Height(height)
		{
		}
	};
}
