#include "ccpch.h"
#include "AudioClip.h"
#include "AudioEngine.h"

namespace Cataclysm
{
	AudioClip::AudioClip(const std::string& filepath)
		: m_FilePath(filepath)
	{
		m_Sound = new ma_sound;
		if (ma_sound_init_from_file(AudioEngine::Get(), filepath.c_str(), 0, nullptr, nullptr, m_Sound) != MA_SUCCESS)
		{
			CC_CORE_ERROR("[AudioClip::AudioClip] Failed to load audio clip: {}", filepath);
			delete m_Sound;
			m_Sound = nullptr;
		}
	}

	AudioClip::~AudioClip()
	{
		if (m_Sound)
		{
			ma_sound_uninit(m_Sound);
			delete m_Sound;
		}
	}

	void AudioClip::Play()
	{
		if (m_Sound)
		{
			ma_sound_set_volume(m_Sound, m_Volume);
			ma_sound_start(m_Sound);
		}
	}

	void AudioClip::Pause()
	{
		if (m_Sound) ma_sound_stop(m_Sound);
	}

	void AudioClip::Stop()
	{
		if (m_Sound)
		{
			ma_sound_stop(m_Sound);
			ma_sound_seek_to_pcm_frame(m_Sound, 0);
		}
	}

	void AudioClip::SetLooping(bool loop)
	{
		m_Looping = loop;
		if (m_Sound)
			ma_sound_set_looping(m_Sound, loop);
	}

	void AudioClip::SetVolume(float volume)
	{
		m_Volume = volume;
		if (m_Sound)
			ma_sound_set_volume(m_Sound, volume);
	}
}
