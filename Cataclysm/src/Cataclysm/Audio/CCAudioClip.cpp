#include "ccpch.h"
#include "Cataclysm/Audio/CCAudioClip.h"

#include "Cataclysm/Audio/CCAudioEngine.h"

namespace Cataclysm
{
	CCAudioClip::CCAudioClip(const std::string& filepath)
		: m_FilePath(filepath)
	{
		m_Sound = new ma_sound;
		if (ma_sound_init_from_file(CCAudioEngine::Get(), filepath.c_str(), 0, nullptr, nullptr, m_Sound) != MA_SUCCESS)
		{
			CC_CORE_ERROR("[CCAudioClip::CCAudioClip] Failed to load audio clip: {}", filepath);
			delete m_Sound;
			m_Sound = nullptr;
		}
	}

	CCAudioClip::~CCAudioClip()
	{
		if (m_Sound)
		{
			ma_sound_uninit(m_Sound);
			delete m_Sound;
		}
	}

	void CCAudioClip::Play()
	{
		if (m_Sound)
		{
			ma_sound_set_volume(m_Sound, m_Volume);
			ma_sound_start(m_Sound);
		}
	}

	void CCAudioClip::Pause()
	{
		if (m_Sound) ma_sound_stop(m_Sound);
	}

	void CCAudioClip::Stop()
	{
		if (m_Sound)
		{
			ma_sound_stop(m_Sound);
			ma_sound_seek_to_pcm_frame(m_Sound, 0);
		}
	}

	void CCAudioClip::SetLooping(bool loop)
	{
		m_Looping = loop;
		if (m_Sound)
			ma_sound_set_looping(m_Sound, loop);
	}

	void CCAudioClip::SetVolume(float volume)
	{
		m_Volume = volume;
		if (m_Sound)
			ma_sound_set_volume(m_Sound, volume);
	}
}
