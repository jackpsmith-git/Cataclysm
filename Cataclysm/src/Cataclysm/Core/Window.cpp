#include "ccpch.h"
#include "Cataclysm/Core/Window.h"

#ifdef CC_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Cataclysm
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef CC_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		CC_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}