using System;

namespace Cataclysm
{
	/// <summary>
	/// Component
	/// </summary>
	public abstract class Component : IEquatable<Component>
    {
		/// <summary>
		/// Parent <see cref="Cataclysm.Entity"/>
		/// </summary>
		public Entity Entity { get; internal set; }

		/// <summary>
		/// Resets all properties to their default values
		/// </summary>
		/// <returns><see langword="void"/></returns>
		public abstract void Reset();

		public override bool Equals(object obj)
		{
			if (!(obj is Component other))
				return false;

			return Entity == other.Entity && GetType() == other.GetType();
		}

		public bool Equals(Component other)
		{
			return Entity == other.Entity && GetType() == other.GetType();
		}

		public override int GetHashCode()
		{
			unchecked
			{
				int hash = 17;
				hash = hash * 31 + Entity.GetHashCode();
				hash = hash * 31 + GetType().GetHashCode();
				return hash;
			}
		}

		public static bool operator ==(Component a, Component b) => Equals(a, b);
		public static bool operator !=(Component a, Component b) => !(a == b);
	}
}
