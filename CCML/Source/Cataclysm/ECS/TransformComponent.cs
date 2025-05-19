namespace Cataclysm
{
	/// <summary>
	/// Cataclysm TransformComponent
	/// </summary>
	public class TransformComponent : Component
    {
        public Vec3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vec3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
            }
        }

		public Vec3 Rotation
		{
			get
			{
				InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vec3 rotation);
				return rotation;
			}
			set
			{
				InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
			}
		}

		public Vec3 Scale
		{
			get
			{
				InternalCalls.TransformComponent_GetScale(Entity.ID, out Vec3 scale);
				return scale;
			}
			set
			{
				InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
			}
		}

		public Vec3 GlobalTranslation
		{
			get
			{
				InternalCalls.TransformComponent_GetGlobalTranslation(Entity.ID, out Vec3 translation);
				return translation;
			}
		}

		public Vec3 GlobalRotation
		{
			get
			{
				InternalCalls.TransformComponent_GetGlobalRotation(Entity.ID, out Vec3 rotation);
				return rotation;
			}
		}

		public Vec3 GlobalScale
		{
			get
			{
				InternalCalls.TransformComponent_GetGlobalScale(Entity.ID, out Vec3 scale);
				return scale;
			}
		}

		public void Translate(Vec3 translation)
		{
			Translation += translation;
		}

		public void Rotate(Vec3 rotation)
		{
			Rotation += rotation;
		}

		public void Reset() => InternalCalls.TransformComponent_Reset(Entity.ID);
	}
}
