using Cataclysm.Math;

namespace Cataclysm
{
	/// <summary>
	/// SpriteRenderer component
	/// </summary>
	public class SpriteRenderer : Renderer
	{ 
		public override Vec4 Color
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

		public Texture2D Texture
		{
			set
			{
				InternalCalls.SpriteRendererComponent_SetTexture(Entity.ID, value.ID);
			}
		}

		public override void Reset() => InternalCalls.SpriteRendererComponent_Reset(Entity.ID);
	}
}
