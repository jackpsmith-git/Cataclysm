using Cataclysm.Math;

namespace Cataclysm.Input
{
	/// <summary>
	/// Cataclysm Input
	/// </summary>
    public class Input
    {

		/// <param name="keycode"></param>
		/// <returns><see langword="true"/> if the key is currently down this frame, otherwise <see langword="false"/></returns>
		public static bool IsKeyDown(KeyCode keycode) => InternalCalls.Input_IsKeyDown(keycode);


		/// <param name="mouseCode"></param>
		/// <returns><see langword="true"/> if the mouse button is currently down this frame, otherwise <see langword="false"/></returns>
		public static bool IsMouseButtonDown(MouseCode mouseCode) => InternalCalls.Input_IsMouseButtonDown(mouseCode);

		/// <summary>
		/// Gets the mouse's current world space position
		/// </summary>
		/// <returns></returns>
		public static Vec2 GetMousePosition()
		{
			InternalCalls.Input_GetMousePosition(out Vec2 position);
			return position;
		}
    }
}
