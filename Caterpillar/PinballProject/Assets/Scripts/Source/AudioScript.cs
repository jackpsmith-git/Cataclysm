using Cataclysm;

namespace Pinball
{
	public class AudioScript : Entity
	{
		private AudioSourceComponent audioSource;

		public void OnCreate()
		{
			audioSource = GetComponent<AudioSourceComponent>();
			audioSource.Play();
		}
	}
}
