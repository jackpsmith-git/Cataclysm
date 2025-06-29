using Cataclysm.Math;

namespace Cataclysm
{
	/// <summary>
	/// BoxCollider2D component
	/// </summary>
	public class BoxCollider2D : Collider2D
	{
		/// <summary>
		/// Collider offset
		/// </summary>
		public override Vec2 Offset
		{
			get
			{
				InternalCalls.BoxCollider2DComponent_GetOffset(Entity.ID, out Vec2 offset);
				return offset;
			}
			set
			{
				InternalCalls.BoxCollider2DComponent_SetOffset(Entity.ID, ref value);
			}
		}

		/// <summary>
		/// Collider size
		/// </summary>
		public Vec2 Size
		{
			get
			{
				InternalCalls.BoxCollider2DComponent_GetSize(Entity.ID, out Vec2 size);
				return size;
			}
			set
			{
				InternalCalls.BoxCollider2DComponent_SetSize(Entity.ID, ref value);
			}
		}

		public override float Density
		{
			get => InternalCalls.BoxCollider2DComponent_GetDensity(Entity.ID);
			set => InternalCalls.BoxCollider2DComponent_SetDensity(Entity.ID, value);
		}

		/// <summary>
		/// Surface friction
		/// </summary>
		public override float Friction
		{
			get => InternalCalls.BoxCollider2DComponent_GetFriction(Entity.ID);
			set => InternalCalls.BoxCollider2DComponent_SetFriction(Entity.ID, value);
		}

		/// <summary>
		/// Surface restitution
		/// </summary>
		public override float Restitution
		{
			get => InternalCalls.BoxCollider2DComponent_GetRestitution(Entity.ID);
			set => InternalCalls.BoxCollider2DComponent_SetRestitution(Entity.ID, value);
		}

		/// <summary>
		/// Surface restitution threshold
		/// </summary>
		public override float RestitutionThreshold
		{
			get => InternalCalls.BoxCollider2DComponent_GetRestitutionThreshold(Entity.ID);
			set => InternalCalls.BoxCollider2DComponent_SetRestitutionThreshold(Entity.ID, value);
		}

		public override bool IsTrigger
		{
			get => InternalCalls.BoxCollider2DComponent_GetIsTrigger(Entity.ID);
			set => InternalCalls.BoxCollider2DComponent_SetIsTrigger(Entity.ID, value);
		}

		public override void Reset() => InternalCalls.BoxCollider2DComponent_Reset(Entity.ID);
	}
}
