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
        public static bool IsKeyDown(KeyCode keycode) => InternalCalls.Input_IsKeyDown(keycode);

		/// <summary>
		/// Checks if a given mouse button is down
		/// </summary>
		/// <param name="mouseCode">Cataclysm Mouse Code</param>
		/// <returns>True if the key is currently down, otherwise false</returns>
		public static bool IsMouseButtonDown(MouseCode mouseCode) => InternalCalls.Input_IsMouseButtonDown(mouseCode);

		/// <summary>
		/// Gets the mouse's current world space position
		/// </summary>
		/// <returns>Vec2</returns>
		public static Vec2 GetMousePosition()
		{
			InternalCalls.Input_GetMousePosition(out Vec2 position);
			return position;
		}
    }
}
