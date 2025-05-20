#include "ccpch.h"
#include "AudioEngine.h"

namespace Cataclysm
{
	ma_engine AudioEngine::s_Engine;
	bool AudioEngine::s_Initialized = false;

	void AudioEngine::Init()
	{
		if (!s_Initialized)
		{
			if (ma_engine_init(nullptr, &s_Engine) != MA_SUCCESS)
			{
				CC_CORE_ERROR("Failed to initialize MiniAudio engine!");
				return;
			}
			s_Initialized = true;
		}
	}

	void AudioEngine::Shutdown()
	{
		if (s_Initialized)
		{
			ma_engine_uninit(&s_Engine);
			s_Initialized = false;
		}
	}

	ma_engine* AudioEngine::Get()
	{
		return s_Initialized ? &s_Engine : nullptr;
	}

	Ref<AudioClip> AudioEngine::LoadClip(const std::string& filepath)
	{
		return CreateRef<AudioClip>(filepath);
	}

	void AudioEngine::Play(Ref<AudioClip> clip)
	{
		if (clip && clip->IsValid())
			clip->Play();
	}

	void AudioEngine::Pause(Ref<AudioClip> clip)
	{
		if (clip && clip->IsValid())
			clip->Pause();
	}

	void AudioEngine::Stop(Ref<AudioClip> clip)
	{
		if (clip && clip->IsValid())
			clip->Stop();
	}
}
