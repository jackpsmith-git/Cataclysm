#pragma once

#include "Cataclysm/Core/CCWindow.h"
#include "Cataclysm/Vesuvius/VSGraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Cataclysm
{
	class CCWindowsWindow : public CCWindow
	{
	public:
		CCWindowsWindow(const CCWindowProps& props);
		virtual ~CCWindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const CCEventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }

		virtual void SetWindowTitle(const char* title) override;
	private:
		virtual void Init(const CCWindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		CCScope<Vesuvius::VSGraphicsContext> m_Context;

		struct CCWindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			CCEventCallbackFn EventCallback;
		};

		CCWindowData m_Data;
	};
}
