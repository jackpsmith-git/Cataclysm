namespace Cataclysm
{
	/// <summary>
	/// Cataclysm Rigidbody2DComponent
	/// </summary>
	public class Rigidbody2DComponent : Component
    {
		/// <summary>
		/// Rigidbody Type
		/// </summary>
		public enum BodyType { Static = 0, Dynamic, Kinematic }

		/// <summary>
		/// Linear Velocity
		/// </summary>
		public Vec2 Velocity
		{
			get
			{
				InternalCalls.Rigidbody2DComponent_GetLinearVelocity(Entity.ID, out Vec2 velocity);
				return velocity;
			}
			set
			{
				InternalCalls.Rigidbody2DComponent_SetLinearVelocity(Entity.ID, ref value);
			}
		}

		public BodyType Type
		{
			get => InternalCalls.Rigidbody2DComponent_GetType(Entity.ID);
			set => InternalCalls.Rigidbody2DComponent_SetType(Entity.ID, value);
		}

		public void ApplyLinearImpulse(Vec2 impulse, Vec2 worldPosition, bool wake)
        {
            InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref worldPosition, wake);
        }

        public void ApplyLinearImpulseToCenter(Vec2 impulse, bool wake)
        {
            InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
        }
    }
}
