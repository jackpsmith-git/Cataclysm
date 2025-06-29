#include "ccpch.h"
#include "Platform/Windows/CCWindowsWindow.h"

#include "Cataclysm/Input/CCInput.h"

#include "Cataclysm/Events/CCApplicationEvent.h"
#include "Cataclysm/Events/CCKeyEvent.h"
#include "Cataclysm/Events/CCMouseEvent.h"

#include "Cataclysm/Vesuvius/VSRenderer.h"

#include "Platform/OpenGL/VSOpenGLContext.h"
#include "Platform/Windows/CataclysmLogo.h"

#include <stb_image.h>
#include <direct.h>

namespace Cataclysm
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		CC_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	CCScope<CCWindow> CCWindow::Create(const CCWindowProps& props)
	{
		return CreateScope<CCWindowsWindow>(props);
	}

	CCWindowsWindow::CCWindowsWindow(const CCWindowProps& props)
	{
		Init(props);
	}

	CCWindowsWindow::~CCWindowsWindow()
	{
		Shutdown();
	}

	void CCWindowsWindow::Init(const CCWindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		// CC_CORE_INFO("[CCWindowsWindow::Init] Creating GLFW window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			CC_CORE_ASSERT(success, "[CCWindowsWindow::Init] Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);

			// glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Uncomment to disable glfw title bar
		}

		{
#if defined(HZ_DEBUG)
			if (VSRenderer::GetAPI() == VSRendererAPI::VSAPI::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_Context = Vesuvius::VSGraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		GLFWimage icon;
		icon.pixels = stbi_load_from_memory(CataclysmLogo_png, CataclysmLogo_png_len, &icon.width, &icon.height, 0, 4);

		if (!icon.pixels)
		{
			CC_CORE_ERROR("[CCWindowsWindow::Init] Window icon contains no pixel data!");
		}
		else
		{
			glfwSetWindowIcon(m_Window, 1, &icon);
			stbi_image_free(icon.pixels);
		}

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				CCWindowData& data = *(CCWindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				CCWindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				CCWindowData& data = *(CCWindowData*)glfwGetWindowUserPointer(window);
				CCWindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				CCWindowData& data = *(CCWindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					CCKeyPressedEvent event(static_cast<CCKeyCode>(key), 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					CCKeyReleasedEvent event(static_cast<CCKeyCode>(key));
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					CCKeyPressedEvent event(static_cast<CCKeyCode>(key), true);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				CCWindowData& data = *(CCWindowData*)glfwGetWindowUserPointer(window);

				CCKeyTypedEvent event(static_cast<CCKeyCode>(keycode));
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				CCWindowData& data = *(CCWindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					CCMouseButtonPressedEvent event(static_cast<CCMouseCode>(button));
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					CCMouseButtonReleasedEvent event(static_cast<CCMouseCode>(button));
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				CCWindowData& data = *(CCWindowData*)glfwGetWindowUserPointer(window);

				CCMouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				CCWindowData& data = *(CCWindowData*)glfwGetWindowUserPointer(window);

				CCMouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	void CCWindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);

		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void CCWindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void CCWindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool CCWindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void CCWindowsWindow::SetWindowTitle(const char* title)
	{
		glfwSetWindowTitle((GLFWwindow*)GetNativeWindow(), title);
	}
}
