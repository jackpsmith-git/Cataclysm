namespace Cataclysm
{
	/// <summary>
	/// Texture2D asset
	/// </summary>
	public class Texture2D
	{
		protected Texture2D() { ID = 0; }
		internal Texture2D(ulong id) => ID = id;
		static internal Texture2D CreateInternal(ulong id) => new Texture2D(id);

		public readonly ulong ID;

		public override string ToString() => ID.ToString();
		public static bool operator ==(Texture2D a, Texture2D b) => a.ID == b.ID;
		public static bool operator !=(Texture2D a, Texture2D b) => a.ID != b.ID;
		public bool Equals(Texture2D other) => ID == other.ID;

		public static implicit operator ulong(Texture2D tex) => tex?.ID ?? 0;
		public static implicit operator Texture2D(ulong id) => new Texture2D(id);
		public static implicit operator bool(Texture2D texture) => texture != null && texture.ID != 0;

		public override bool Equals(object obj)
		{
			if (obj == null) return false;
			if (!(obj is Texture2D)) return false;

			Texture2D texture2D = (Texture2D)obj;
			return ID == texture2D.ID;
		}

		public override int GetHashCode()
		{
			return 1213502048 + ID.GetHashCode();
		}
	}
}
