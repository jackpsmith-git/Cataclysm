namespace Cataclysm
{
	/// <summary>
	/// Cataclysm Component
	/// </summary>
	public abstract class Component
    {
		/// <summary>
		/// Parent entity of the component
		/// </summary>
        public Entity Entity { get; internal set; }
    }
}
