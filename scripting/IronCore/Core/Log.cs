using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Logging methods
    /// </summary>
    public static class Log
    {
        /// <summary>
        /// Log debug message
        /// </summary>
        /// <param name="message">Message</param>
        public static void LogDebug(string message)
        {
            LogDebug_Internal(message);
        }
        
        /// <summary>
        /// Log info message
        /// </summary>
        /// <param name="message">Message</param>
        public static void LogInfo(string message)
        {
            LogInfo_Internal(message);
        }
        
        /// <summary>
        /// Log warning message
        /// </summary>
        /// <param name="message">Message</param>
        public static void LogWarning(string message)
        {
            LogWarning_Internal(message);
        }
        
        /// <summary>
        /// Log error message
        /// </summary>
        /// <param name="message">Message</param>
        public static void LogError(string message)
        {
            LogError_Internal(message);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LogDebug_Internal(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LogInfo_Internal(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LogWarning_Internal(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LogError_Internal(string message);
    }
}