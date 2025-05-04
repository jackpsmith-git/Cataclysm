#pragma once

#include "Cataclysm.h"

namespace Cataclysm
{
	class FrameDebuggerPanel
	{
	public:
		FrameDebuggerPanel() = default;
		
		void OnImGuiRender(Timestep ts);
	private:
		float samples[100];
	};
}
