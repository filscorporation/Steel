using System.Runtime.CompilerServices;

namespace Iron
{
    public static class Debug
    {
        public static void EnableDebugInfoWindow() => EnableDebugInfoWindow_Internal();
        
        public static void DisableDebugInfoWindow() => DisableDebugInfoWindow_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void EnableDebugInfoWindow_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void DisableDebugInfoWindow_Internal();
    }
}