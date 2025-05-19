namespace Cataclysm
{
	/// <summary>
	/// Cataclysm SpriteRendererComponent
	/// </summary>
	public class SpriteRendererComponent : Component
	{ 
		public Vec4 Color
		{
			get
			{
				InternalCalls.SpriteRendererComponent_GetColor(Entity.ID, out Vec4 color);
				return color;
			}
			set
			{
				InternalCalls.SpriteRendererComponent_SetColor(Entity.ID, ref value);
			}
		}

		public float TilingFactor
		{
			get => InternalCalls.SpriteRendererComponent_GetTilingFactor(Entity.ID);
			set => InternalCalls.SpriteRendererComponent_SetTilingFactor(Entity.ID, value);
		}

		public void Reset() => InternalCalls.SpriteRendererComponent_Reset(Entity.ID);
	}
}
