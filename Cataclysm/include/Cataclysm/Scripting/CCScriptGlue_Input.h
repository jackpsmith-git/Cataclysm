#pragma once

#include "CCScriptEngine.h"

namespace Cataclysm
{
	static bool Input_IsKeyDown(CCKeyCode keycode)
	{
		return CCInput::IsKeyDown(keycode);
	}

	static bool Input_IsMouseButtonDown(CCMouseCode button)
	{
		return CCInput::IsMouseButtonDown(button);
	}

	static void Input_GetMousePosition(glm::vec2* outPosition)
	{
		*outPosition = glm::vec2(CCInput::GetMouseX(), CCInput::GetMouseY());
	}
}
