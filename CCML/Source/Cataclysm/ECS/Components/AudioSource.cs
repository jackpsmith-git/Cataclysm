namespace Cataclysm
{
	/// <summary>
	/// AudioSource component
	/// </summary>
	public class AudioSource : Component
	{
		/// <summary>
		/// AudioClip asset
		/// </summary>
		public AudioClip AudioClip
		{
			set
			{
				InternalCalls.AudioSourceComponent_SetAudioClip(Entity.ID, value.ID);
			}
		}

		/// <summary>
		/// Starts playing the audio clip
		/// </summary>
		/// <returns><see langword="void"/></returns>
		public void Play() => InternalCalls.AudioSourceComponent_Play(Entity.ID);

		/// <summary>
		/// Pauses the audio clip
		/// </summary>
		/// <returns><see langword="void"/></returns>
		public void Pause() => InternalCalls.AudioSourceComponent_Pause(Entity.ID);

		/// <summary>
		/// Stops playing the audio clip
		/// </summary>
		/// <returns><see langword="void"/></returns>
		public void Stop() => InternalCalls.AudioSourceComponent_Stop(Entity.ID);

		public override void Reset() {}
	}
}
