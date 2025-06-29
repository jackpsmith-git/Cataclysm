#pragma once

#include "Cataclysm.h"
#include "Cataclysm/Core/CCCore.h"
#include "Cataclysm/Core/CCLog.h"
#include "Cataclysm/Scene/CCScene.h"

namespace Cataclysm
{
	class CPFrameDebuggerPanel
	{
	public:
		CPFrameDebuggerPanel() = default;

		void OnImGuiRender(CCTimestep ts);
	private:
		float samples[100];
	};
}
