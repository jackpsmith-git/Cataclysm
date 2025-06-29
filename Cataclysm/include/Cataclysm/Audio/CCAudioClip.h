#pragma once

#include "Cataclysm.h"
#include "miniaudio.h"

namespace Cataclysm
{
	class CCAudioClip
	{
	public:
		CCAudioClip::CCAudioClip(const std::string& filepath);
		CCAudioClip::~CCAudioClip();
		
		void CCAudioClip::Play();
		void CCAudioClip::Pause();
		void CCAudioClip::Stop();

		void CCAudioClip::SetLooping(bool loop);
		bool CCAudioClip::IsValid() const { return m_Sound != nullptr; }

		void SetVolume(float volume);
		float GetVolume() const { return m_Volume; }

		std::string GetFilePath() { return m_FilePath; }
	private:
		ma_sound* m_Sound;
		float m_Volume = 1.0f;
		bool m_Looping = false;
		std::string m_FilePath;
	};
}
