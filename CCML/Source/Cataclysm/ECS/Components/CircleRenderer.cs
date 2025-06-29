using Cataclysm.Math;

namespace Cataclysm
{
	/// <summary>
	/// CircleRenderer component
	/// </summary>
	public class CircleRenderer : Renderer
	{
		/// <summary>
		/// Vertex color
		/// </summary>
		public override Vec4 Color
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

		public override void Reset() => InternalCalls.CircleRendererComponent_Reset(Entity.ID);
	}
}
