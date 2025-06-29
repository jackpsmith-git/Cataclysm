#pragma once

#include "Cataclysm.h"

namespace Cataclysm
{
	class CPWelcomePanel
	{
	public:
		CPWelcomePanel();

		void OnImGuiRender(bool& panelEnabled);
	};
}
