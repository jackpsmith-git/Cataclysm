using Cataclysm.Math;

namespace Cataclysm
{
	/// <summary>
	/// CircleCollider2D component
	/// </summary>
	public class CircleCollider2D : Collider2D
	{
		/// <summary>
		/// Collider offset
		/// </summary>
		public override Vec2 Offset
		{
			get
			{
				InternalCalls.CircleCollider2DComponent_GetOffset(Entity.ID, out Vec2 offset);
				return offset;
			}
			set
			{
				InternalCalls.CircleCollider2DComponent_SetOffset(Entity.ID, ref value);
			}
		}

		/// <summary>
		/// Collider radius
		/// </summary>
		public float Radius
		{
			get => InternalCalls.CircleCollider2DComponent_GetRadius(Entity.ID);
			set => InternalCalls.CircleCollider2DComponent_SetRadius(Entity.ID, value);
		}

		public override float Density
		{
			get => InternalCalls.CircleCollider2DComponent_GetDensity(Entity.ID);
			set => InternalCalls.CircleCollider2DComponent_SetDensity(Entity.ID, value);
		}

		/// <summary>
		/// Surface friction
		/// </summary>
		public override float Friction
		{
			get => InternalCalls.CircleCollider2DComponent_GetFriction(Entity.ID);
			set => InternalCalls.CircleCollider2DComponent_SetFriction(Entity.ID, value);
		}

		/// <summary>
		/// Surface restitution
		/// </summary>
		public override float Restitution
		{
			get => InternalCalls.CircleCollider2DComponent_GetRestitution(Entity.ID);
			set => InternalCalls.CircleCollider2DComponent_SetRestitution(Entity.ID, value);
		}

		/// <summary>
		/// Surface restitution threshold
		/// </summary>
		public override float RestitutionThreshold
		{
			get => InternalCalls.CircleCollider2DComponent_GetRestitutionThreshold(Entity.ID);
			set => InternalCalls.CircleCollider2DComponent_SetRestitutionThreshold(Entity.ID, value);
		}

		public override bool IsTrigger
		{
			get => InternalCalls.CircleCollider2DComponent_GetIsTrigger(Entity.ID);
			set => InternalCalls.CircleCollider2DComponent_SetIsTrigger(Entity.ID, value);
		}

		public override void Reset() => InternalCalls.CircleCollider2DComponent_Reset(Entity.ID);
	}
}
