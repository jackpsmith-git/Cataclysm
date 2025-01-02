namespace Cataclysm
{
	/// <summary>
	/// Cataclysm Input
	/// </summary>
    public class Input
    {
		/// <summary>
		/// Checks if a given key is down
		/// </summary>
		/// <param name="keycode">Cataclysm Key Code</param>
		/// <returns>True if the key is currently down, otherwise false</returns>
        public static bool IsKeyDown(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyDown(keycode);
        }
    }
}
