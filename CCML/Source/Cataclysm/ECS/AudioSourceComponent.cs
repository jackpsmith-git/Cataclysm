namespace Cataclysm
{
	public class AudioSourceComponent : Component
	{
		public void Play() => InternalCalls.AudioSourceComponent_Play(Entity.ID);
		public void Pause() => InternalCalls.AudioSourceComponent_Pause(Entity.ID);
		public void Stop() => InternalCalls.AudioSourceComponent_Stop(Entity.ID);
	}
}
