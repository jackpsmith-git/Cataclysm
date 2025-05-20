#pragma once

#include "miniaudio.h"
#include "AudioClip.h"

namespace Cataclysm
{
	struct AudioHandle
	{
		void* Data = nullptr;
	};

	class AudioEngine
	{
	public:
		static void Init();
		static void Shutdown();
		static ma_engine* Get();

		static Ref<AudioClip> LoadClip(const std::string& filepath);
		static void Play(Ref<AudioClip> clip);
		static void Pause(Ref<AudioClip> clip);
		static void Stop(Ref<AudioClip> clip);
	private:
		static ma_engine s_Engine;
		static bool s_Initialized;
	};
}
