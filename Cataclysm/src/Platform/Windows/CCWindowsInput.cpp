#include "ccpch.h"
#include "Cataclysm/Input/CCInput.h"

#include "GLFW/glfw3.h"
#include "Cataclysm/Core/CCApplication.h"

namespace Cataclysm
{
	bool CCInput::IsKeyDown(CCKeyCode key)
	{
		auto window = static_cast<GLFWwindow*>(CCApplication::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}

	bool CCInput::IsMouseButtonDown(CCMouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(CCApplication::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	std::pair<float, float> CCInput::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(CCApplication::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float CCInput::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float CCInput::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}
