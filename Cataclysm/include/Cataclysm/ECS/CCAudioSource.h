#pragma once

namespace Cataclysm
{
	class CCAudioClip;

	struct AudioSource
	{
		bool SceneManaged = false;
		bool WasPlayingBeforePause = false;

		std::string FilePath;
		CCRef<CCAudioClip> AudioClip = nullptr;

		float Volume = 1.0f;
		bool Loop = false;
		bool PlayOnStart = false;
		bool IsPlaying = false;

		AudioSource() = default;
		AudioSource(const AudioSource&) = default;

		void Reset()
		{
			FilePath.clear();
			Loop = false;
			PlayOnStart = false;
			AudioClip = nullptr;
			IsPlaying = false;
			Volume = 1.0f;
		}
	};
}
