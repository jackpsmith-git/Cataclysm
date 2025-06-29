namespace Cataclysm
{
	/// <summary>
	/// Font asset
	/// </summary>
	public class Font
	{
		protected Font() { ID = 0; }
		internal Font(ulong id) => ID = id;
		static internal Font CreateInternal(ulong id) => new Font(id);

		public readonly ulong ID;

		public override string ToString() => ID.ToString();
		public static bool operator ==(Font a, Font b) => a.ID == b.ID;
		public static bool operator !=(Font a, Font b) => a.ID != b.ID;
		public bool Equals(Font other) => ID == other.ID;

		public static implicit operator ulong(Font font) => font?.ID ?? 0;
		public static implicit operator Font(ulong id) => new Font(id);
		public static implicit operator bool(Font font) => font != null && font.ID != 0;

		public override bool Equals(object obj)
		{
			if (obj == null) return false;
			if (!(obj is Font)) return false;
			
			Font font = (Font)obj;
			return ID == font.ID;
		}

		public override int GetHashCode()
		{
			return 1213502048 + ID.GetHashCode();
		}
	}
}
