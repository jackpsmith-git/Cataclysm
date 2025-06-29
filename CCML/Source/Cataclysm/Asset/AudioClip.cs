namespace Cataclysm
{
	/// <summary>
	/// AudioClip asset
	/// </summary>
	public class AudioClip
	{
		protected AudioClip() { ID = 0; }
		internal AudioClip(ulong id) => ID = id;
		static internal AudioClip CreateInternal(ulong id) => new AudioClip(id);

		public readonly ulong ID;

		public override string ToString() => ID.ToString();
		public static bool operator ==(AudioClip a, AudioClip b) => a.ID == b.ID;
		public static bool operator !=(AudioClip a, AudioClip b) => a.ID != b.ID;
		public bool Equals(AudioClip other) => ID == other.ID;

		public static implicit operator ulong(AudioClip clip) => clip?.ID ?? 0;
		public static implicit operator AudioClip(ulong id) => new AudioClip(id);
		public static implicit operator bool(AudioClip clip) => clip != null && clip.ID != 0;

		public override bool Equals(object obj)
		{
			if (obj == null) return false;
			if (!(obj is AudioClip)) return false;

			AudioClip clip = (AudioClip)obj;
			return ID == clip.ID;
		}

		public override int GetHashCode()
		{
			return 1213502048 + ID.GetHashCode();
		}
	}
}
