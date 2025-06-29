#pragma once

#include "ccpch.h"

#include "Cataclysm/Core/CCCore.h"
#include "Cataclysm/Core/CCWindowProps.h"

#include "Cataclysm/Events/CCEvent.h"

#include <sstream>

namespace Cataclysm
{
	class CCWindow
	{
	public:
		using CCEventCallbackFn = std::function<void(CCEvent&)>;

		virtual ~CCWindow() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const CCEventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void SetWindowTitle(const char* title) = 0;

		static CCScope<CCWindow> Create(const CCWindowProps& props = CCWindowProps());
	};
}
