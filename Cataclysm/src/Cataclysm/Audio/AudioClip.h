#pragma once

#include "Cataclysm.h"
#include "miniaudio.h"

namespace Cataclysm
{
	class AudioClip
	{
	public:
		AudioClip::AudioClip(const std::string& filepath);
		AudioClip::~AudioClip();
		
		void AudioClip::Play();
		void AudioClip::Pause();
		void AudioClip::Stop();

		void AudioClip::SetLooping(bool loop);
		bool AudioClip::IsValid() const { return m_Sound != nullptr; }

		void SetVolume(float volume);
		float GetVolume() const { return m_Volume; }
	private:
		ma_sound* m_Sound;
		float m_Volume = 1.0f;
		bool m_Looping = false;
		std::string m_FilePath;
	};
}
