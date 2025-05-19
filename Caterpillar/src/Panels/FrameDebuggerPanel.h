#pragma once

#include "Cataclysm.h"
#include "Cataclysm/Core/Core.h"
#include "Cataclysm/Core/Log.h"
#include "Cataclysm/Scene/Scene.h"

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
