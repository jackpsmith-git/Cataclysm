#pragma once

#include "Cataclysm/Core/CCCore.h"
#include "Cataclysm/Input/CCKeyCode.h"
#include "Cataclysm/Input/CCMouseCode.h"

namespace Cataclysm {

	class CCInput
	{
	public:
		static bool IsKeyDown(CCKeyCode key);
		static bool IsMouseButtonDown(CCMouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
