#include "ccpch.h"
#include "Cataclysm/Core/CCWindow.h"

#ifdef CC_PLATFORM_WINDOWS
#include "Platform/Windows/CCWindowsWindow.h"
#endif

namespace Cataclysm
{
	CCScope<CCWindow> CCWindow::Create(const CCWindowProps& props)
	{
#ifdef CC_PLATFORM_WINDOWS
		return CreateScope<CCWindowsWindow>(props);
#else
		CC_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}
