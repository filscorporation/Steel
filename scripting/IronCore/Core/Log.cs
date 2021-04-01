using System.Runtime.CompilerServices;

namespace Iron
{
    public static class Log
    {
        public static void LogInfo(string message)
        {
            LogInfo_Internal(message);
        }
        
        public static void LogWarning(string message)
        {
            LogWarning_Internal(message);
        }
        
        public static void LogError(string message)
        {
            LogError_Internal(message);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LogInfo_Internal(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LogWarning_Internal(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LogError_Internal(string message);
    }
}