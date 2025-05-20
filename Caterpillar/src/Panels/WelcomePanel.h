#pragma once

#include "Cataclysm.h"

namespace Cataclysm
{
	class WelcomePanel
	{
	public:
		WelcomePanel();

		void OnImGuiRender(bool& panelEnabled);
	};
}
