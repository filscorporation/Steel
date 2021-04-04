using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Debugging functions
    /// </summary>
    public static class Debug
    {
        /// <summary>
        /// Shows debugging window with information and debugging options buttons
        /// </summary>
        public static void EnableDebugInfoWindow() => EnableDebugInfoWindow_Internal();
        
        /// <summary>
        /// Hide debugging window
        /// </summary>
        public static void DisableDebugInfoWindow() => DisableDebugInfoWindow_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void EnableDebugInfoWindow_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void DisableDebugInfoWindow_Internal();
    }
}