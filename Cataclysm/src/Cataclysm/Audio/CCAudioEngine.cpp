#include "ccpch.h"
#include "Cataclysm/Audio/CCAudioEngine.h"

namespace Cataclysm
{
	ma_engine CCAudioEngine::s_Engine;
	bool CCAudioEngine::s_Initialized = false;

	void CCAudioEngine::Init()
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

	void CCAudioEngine::Shutdown()
	{
		if (s_Initialized)
		{
			ma_engine_uninit(&s_Engine);
			s_Initialized = false;
		}
	}

	ma_engine* CCAudioEngine::Get()
	{
		return s_Initialized ? &s_Engine : nullptr;
	}

	CCRef<CCAudioClip> CCAudioEngine::LoadClip(const std::string& filepath)
	{
		return CreateRef<CCAudioClip>(filepath);
	}

	void CCAudioEngine::Play(CCRef<CCAudioClip> clip)
	{
		if (clip && clip->IsValid())
			clip->Play();
	}

	void CCAudioEngine::Pause(CCRef<CCAudioClip> clip)
	{
		if (clip && clip->IsValid())
			clip->Pause();
	}

	void CCAudioEngine::Stop(CCRef<CCAudioClip> clip)
	{
		if (clip && clip->IsValid())
			clip->Stop();
	}
}
