using Cataclysm.Math;

namespace Cataclysm
{
	/// <summary>
	/// Rigidbody2D component
	/// </summary>
	public class Rigidbody2D : Component
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

		/// <summary>
		/// Applies a force at a given point in world space
		/// </summary>
		/// <param name="force"></param>
		/// <param name="point"></param>
		/// <param name="wake"></param>
		/// <returns><see langword="void"/></returns>
		public void ApplyForce(Vec2 force, Vec2 point, bool wake) => InternalCalls.Rigidbody2DComponent_ApplyForce(Entity.ID, ref force, ref point, wake);

		/// <summary>
		/// Applies a force at the center of the rigidbody
		/// </summary>
		/// <param name="force"></param>
		/// <param name="wake"></param>
		/// <returns><see langword="void"/></returns>
		public void ApplyForce(Vec2 force, bool wake) => InternalCalls.Rigidbody2DComponent_ApplyForceToCenter(Entity.ID, ref force, wake);

		/// <summary>
		/// Applies torque to the rigidbody
		/// </summary>
		/// <param name="torque"></param>
		/// <param name="wake"></param>
		/// <returns><see langword="void"/></returns>
		public void ApplyTorque(float torque, bool wake) => InternalCalls.Rigidbody2DComponent_ApplyTorque(Entity.ID, torque, wake);

		/// <summary>
		/// Applies a linear impulse at a given point in world space
		/// </summary>
		/// <param name="impulse"></param>
		/// <param name="worldPosition"></param>
		/// <param name="wake"></param>
		/// <returns><see langword="void"/></returns>
		public void ApplyLinearImpulse(Vec2 impulse, Vec2 worldPosition, bool wake) => InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref worldPosition, wake);

		/// <summary>
		/// Applies a linear impulse at the center of the rigidbody
		/// </summary>
		/// <param name="impulse"></param>
		/// <param name="wake"></param>
		/// <returns><see langword="void"/></returns>
		public void ApplyLinearImpulse(Vec2 impulse, bool wake) => InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);

		/// <summary>
		/// Applies an angular impulse to the rigidbody
		/// </summary>
		/// <param name="impulse"></param>
		/// <param name="wake"></param>
		/// <returns><see langword="void"/></returns>
		public void ApplyAngularImpulse(float impulse, bool wake) => InternalCalls.Rigidbody2DComponent_ApplyAngularImpulse(Entity.ID, impulse, wake);

		public override void Reset() => InternalCalls.Rigidbody2DComponent_Reset(Entity.ID);
	}
}
