#pragma once

#include "Cataclysm/Core/Core.h"
#include "Cataclysm/Input/KeyCodes.h"
#include "Cataclysm/Input/MouseCodes.h"

namespace Cataclysm {

	class Input
	{
	public:
		static bool IsKeyDown(KeyCode key);
		static bool IsMouseButtonDown(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
