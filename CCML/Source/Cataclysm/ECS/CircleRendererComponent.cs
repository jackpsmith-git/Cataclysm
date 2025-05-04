namespace Cataclysm
{
	/// <summary>
	/// Cataclysm CircleRendererComponent
	/// </summary>
	public class CircleRendererComponent : Component
	{
		/// <summary>
		/// Vertex color
		/// </summary>
		public Vec4 Color
		{
			get
			{
				InternalCalls.CircleRendererComponent_GetColor(Entity.ID, out Vec4 color);
				return color;
			}
			set
			{
				InternalCalls.CircleRendererComponent_SetColor(Entity.ID, ref value);
			}
		}

		/// <summary>
		/// Border thickenss
		/// </summary>
		public float Thickness
		{
			get => InternalCalls.CircleRendererComponent_GetThickness(Entity.ID);
			set => InternalCalls.CircleRendererComponent_SetThickness(Entity.ID, value);
		}

		/// <summary>
		/// Border fade
		/// </summary>
		public float Fade
		{
			get => InternalCalls.CircleRendererComponent_GetFade(Entity.ID);
			set => InternalCalls.CircleRendererComponent_SetFade(Entity.ID, value);
		}
	}
}
