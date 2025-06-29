#pragma once

#include "Cataclysm.h"

namespace Cataclysm
{
	class CPOutputPanel
	{
	public:
		CPOutputPanel() = default;
		void OnImGuiRender();
	private:
		bool warningsEnabled = true;
		bool errorsEnabled = true;
		bool traceEnabled = true;
		bool infoEnabled = true;
		bool m_ShowTimestamps = true;
	};
}
