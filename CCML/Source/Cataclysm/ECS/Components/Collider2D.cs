using Cataclysm.Math;

namespace Cataclysm
{
	/// <summary>
	/// Collider2D component
	/// </summary>
	public abstract class Collider2D : Component
	{
		public abstract Vec2 Offset { get; set; }
		public abstract float Density { get; set; }
		public abstract float Friction { get; set; }
		public abstract float Restitution { get; set; }
		public abstract float RestitutionThreshold { get; set; }
		public abstract bool IsTrigger { get; set; }

		public abstract override void Reset();
	}
}
