#pragma once

#include "Cataclysm/Scene/CCScene.h"

namespace Cataclysm
{
	class CCSceneAudioManager
	{
	public:
		static void OnAudioStop(CCScene* scene);
		static void ProcessAudio(CCScene* scene);
	};
}
