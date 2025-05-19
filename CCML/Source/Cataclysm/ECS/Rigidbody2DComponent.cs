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

		public float GravityScale
		{
			get => InternalCalls.Rigidbody2DComponent_GetGravityScale(Entity.ID);
			set => InternalCalls.Rigidbody2DComponent_SetGravityScale(Entity.ID, value);
		}

		public float Mass
		{
			get => InternalCalls.Rigidbody2DComponent_GetMass(Entity.ID);
			set => InternalCalls.Rigidbody2DComponent_SetMass(Entity.ID, value);
		}

		public void ApplyForce(Vec2 force, Vec2 point, bool wake) => InternalCalls.Rigidbody2DComponent_ApplyForce(Entity.ID, ref force, ref point, wake);
		public void ApplyForce(Vec2 force, bool wake) => InternalCalls.Rigidbody2DComponent_ApplyForceToCenter(Entity.ID, ref force, wake);
		public void ApplyTorque(float torque, bool wake) => InternalCalls.Rigidbody2DComponent_ApplyTorque(Entity.ID, torque, wake);
		public void ApplyLinearImpulse(Vec2 impulse, Vec2 worldPosition, bool wake) => InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref worldPosition, wake);
        public void ApplyLinearImpulse(Vec2 impulse, bool wake) => InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
		public void ApplyAngularImpulse(float impulse, bool wake) => InternalCalls.Rigidbody2DComponent_ApplyAngularImpulse(Entity.ID, impulse, wake);

		public void Reset() => InternalCalls.Rigidbody2DComponent_Reset(Entity.ID);
	}
}
