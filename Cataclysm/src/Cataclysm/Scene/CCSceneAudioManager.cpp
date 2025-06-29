#include "ccpch.h"
#include "Cataclysm/Scene/CCSceneAudioManager.h"

#include "Cataclysm/Audio/CCAudioEngine.h"

namespace Cataclysm
{
	void CCSceneAudioManager::OnAudioStop(CCScene* scene)
	{
		auto view = scene->GetRegistry().view<AudioSource>();
		for (auto e : view)
		{
			Entity entity = { e, scene };
			AudioSource& asc = entity.GetComponent<AudioSource>();

			if (!asc.AudioClip && !asc.FilePath.empty())
			{
				asc.AudioClip = CCAudioEngine::LoadClip(asc.FilePath);
				if (asc.AudioClip)
					asc.AudioClip->SetLooping(asc.Loop);
			}

			if (asc.IsPlaying)
			{
				CCAudioEngine::Stop(asc.AudioClip);
				asc.IsPlaying = false;
			}
		}
	}

	void CCSceneAudioManager::ProcessAudio(CCScene* scene)
	{
		if (!scene->IsPaused() || scene->Stepped())
		{
			auto view = scene->GetRegistry().view<AudioSource>();
			for (auto e : view)
			{
				Entity entity = { e, scene };
				AudioSource& asc = entity.GetComponent<AudioSource>();

				if (!asc.AudioClip && !asc.FilePath.empty())
				{
					asc.AudioClip = CCAudioEngine::LoadClip(asc.FilePath);
					if (asc.AudioClip)
						asc.AudioClip->SetLooping(asc.Loop);
				}

				if (asc.PlayOnStart)
				{
					CCAudioEngine::Play(asc.AudioClip);
					asc.IsPlaying = true;
				}
			}
		}
		else
		{
			auto view = scene->GetRegistry().view<AudioSource>();
			for (auto e : view)
			{
				Entity entity = { e, scene };
				AudioSource& asc = entity.GetComponent<AudioSource>();

				if (!asc.AudioClip && !asc.FilePath.empty())
				{
					asc.AudioClip = CCAudioEngine::LoadClip(asc.FilePath);
					if (asc.AudioClip)
						asc.AudioClip->SetLooping(asc.Loop);
				}

				if (asc.IsPlaying)
				{
					CCAudioEngine::Pause(asc.AudioClip);
					asc.IsPlaying = false;
				}
			}
		}
	}
}
