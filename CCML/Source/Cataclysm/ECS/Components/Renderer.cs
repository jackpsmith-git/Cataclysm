using Cataclysm.Math;

namespace Cataclysm
{
	/// <summary>
	/// Renderer component
	/// </summary>
	public abstract class Renderer : Component
	{
		/// <summary>
		/// Vertex color
		/// </summary>
		public abstract Vec4 Color { get; set; }

		public abstract override void Reset();
	}
}
