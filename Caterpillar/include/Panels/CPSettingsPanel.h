#pragma once

#include "Cataclysm.h"

namespace Cataclysm
{
	class CPSettingsPanel
	{
	public:
		CPSettingsPanel();

		void OnImGuiRender(const CCRef<CCScene>& scene);
	};
}
