namespace Cataclysm
{
	/// <summary>
	/// Cataclysm CameraComponent
	/// </summary>
	public class CameraComponent : Component
	{
		public bool Primary
		{
			get => InternalCalls.CameraComponent_GetPrimary(Entity.ID);
			set => InternalCalls.CameraComponent_SetPrimary(Entity.ID, value);
		}

		public bool FixedAspectRatio
		{
			get => InternalCalls.CameraComponent_GetFixedAspectRatio(Entity.ID);
			set => InternalCalls.CameraComponent_SetFixedAspectRatio(Entity.ID, value);
		}

		public float OrthographicSize
		{
			get => InternalCalls.CameraComponent_GetOrthograpicSize(Entity.ID);
			set => InternalCalls.CameraComponent_SetOrthographicSize(Entity.ID, value);
		}

		public float OrthographicNearClip
		{
			get => InternalCalls.CameraComponent_GetOrthographicNearClip(Entity.ID);
			set => InternalCalls.CameraComponent_SetOrthographicNearClip(Entity.ID, value);
		}

		public float OrthographicFarClip
		{
			get => InternalCalls.CameraComponent_GetOrthographicFarClip(Entity.ID);
			set => InternalCalls.CameraComponent_SetOrthographicFarClip(Entity.ID, value);
		}

		public float PerspectiveVerticalFOV
		{
			get => InternalCalls.CameraComponent_GetPerspectiveVerticalFOV(Entity.ID);
			set => InternalCalls.CameraComponent_SetPerspectiveVerticalFOV(Entity.ID, value);
		}

		public float PerspectiveNearClip
		{
			get => InternalCalls.CameraComponent_GetPerspectiveNearClip(Entity.ID);
			set => InternalCalls.CameraComponent_SetPerspectiveNearClip(Entity.ID, value);
		}

		public float PerspectiveFarClip
		{
			get => InternalCalls.CameraComponent_GetPerspectiveFarClip(Entity.ID);
			set => InternalCalls.CameraComponent_SetPerspectiveFarClip(Entity.ID, value);
		}

		public void Reset() => InternalCalls.CameraComponent_Reset(Entity.ID);
	}
}
