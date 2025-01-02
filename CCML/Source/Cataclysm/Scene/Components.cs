namespace Cataclysm
{
	/// <summary>
	/// Cataclysm Component
	/// </summary>
	public abstract class Component
    {
		/// <summary>
		/// Parent entity of the component
		/// </summary>
        public Entity Entity { get; internal set; }
    }

	/// <summary>
	/// Cataclysm Transform Component
	/// </summary>
	public class Transform : Component
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
    }

	/// <summary>
	/// Cataclysm Rigidbody2D Component
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

        public void ApplyLinearImpulse(Vec2 impulse, bool wake)
        {
            InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
        }
    }

	/// <summary>
	/// Cataclysm text Component
	/// </summary>
	public class Text : Component
	{
		public string text
		{
			get => InternalCalls.TextComponent_GetText(Entity.ID);
			set => InternalCalls.TextComponent_SetText(Entity.ID, value);
		}

		public Vec4 Color
		{
			get
			{
				InternalCalls.TextComponent_GetColor(Entity.ID, out Vec4 color);
				return color;
			}
			set
			{
				InternalCalls.TextComponent_SetColor(Entity.ID, ref value);
			}
		}

		public float Kerning
		{
			get => InternalCalls.TextComponent_GetKerning(Entity.ID);
			set => InternalCalls.TextComponent_SetKerning(Entity.ID, value);
		}

		public float LineSpacing
		{
			get => InternalCalls.TextComponent_GetLineSpacing(Entity.ID);
			set => InternalCalls.TextComponent_SetLineSpacing(Entity.ID, value);
		}
	}
}
