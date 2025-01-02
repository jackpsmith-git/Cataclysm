#pragma once

#include "Cataclysm/Core/Core.h"
#include "Cataclysm/Core/KeyCodes.h"
#include "Cataclysm/Core/MouseCodes.h"


namespace Cataclysm {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}