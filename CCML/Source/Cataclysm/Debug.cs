namespace Cataclysm
{
	public static class Debug
	{
		public static void Log(string message) => InternalCalls.Debug_Log(message);
		public static void Info(string message) => InternalCalls.Debug_Info(message);
		public static void Warn(string message) => InternalCalls.Debug_Warn(message);
		public static void Error(string message) => InternalCalls.Debug_Error(message);

		public static void Log(object obj) => InternalCalls.Debug_Log(obj.ToString());
		public static void Info(object obj) => InternalCalls.Debug_Info(obj.ToString());
		public static void Warn(object obj) => InternalCalls.Debug_Warn(obj.ToString());
		public static void Error(object obj) => InternalCalls.Debug_Error(obj.ToString());
	}
}
