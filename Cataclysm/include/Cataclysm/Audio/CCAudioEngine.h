#pragma once

#include "miniaudio.h"
#include "CCAudioClip.h"

namespace Cataclysm
{
	struct CCAudioHandle
	{
		void* Data = nullptr;
	};

	class CCAudioEngine
	{
	public:
		static void Init();
		static void Shutdown();
		static ma_engine* Get();

		static CCRef<CCAudioClip> LoadClip(const std::string& filepath);
		static void Play(CCRef<CCAudioClip> clip);
		static void Pause(CCRef<CCAudioClip> clip);
		static void Stop(CCRef<CCAudioClip> clip);
	private:
		static ma_engine s_Engine;
		static bool s_Initialized;
	};
}
