namespace Cataclysm
{
	/// <summary>
	/// Cataclysm CircleCollider2DComponent
	/// </summary>
	public class CircleCollider2DComponent : Component
	{
		/// <summary>
		/// Collider offset
		/// </summary>
		public Vec2 Offset
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

		/// <summary>
		/// Mass density
		/// </summary>
		public float Density
		{
			get => InternalCalls.CircleCollider2DComponent_GetDensity(Entity.ID);
			set => InternalCalls.CircleCollider2DComponent_SetDensity(Entity.ID, value);
		}

		/// <summary>
		/// Surface friction
		/// </summary>
		public float Friction
		{
			get => InternalCalls.CircleCollider2DComponent_GetFriction(Entity.ID);
			set => InternalCalls.CircleCollider2DComponent_SetFriction(Entity.ID, value);
		}

		/// <summary>
		/// Surface restitution
		/// </summary>
		public float Restitution
		{
			get => InternalCalls.CircleCollider2DComponent_GetRestitution(Entity.ID);
			set => InternalCalls.CircleCollider2DComponent_SetRestitution(Entity.ID, value);
		}

		/// <summary>
		/// Surface restitution threshold
		/// </summary>
		public float RestitutionThreshold
		{
			get => InternalCalls.CircleCollider2DComponent_GetRestitutionThreshold(Entity.ID);
			set => InternalCalls.CircleCollider2DComponent_SetRestitutionThreshold(Entity.ID, value);
		}

		public bool IsTrigger
		{
			get => InternalCalls.CircleCollider2DComponent_GetIsTrigger(Entity.ID);
			set => InternalCalls.CircleCollider2DComponent_SetIsTrigger(Entity.ID, value);
		}

		public void Reset() => InternalCalls.CircleCollider2DComponent_Reset(Entity.ID);
	}
}
