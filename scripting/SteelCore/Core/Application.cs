using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Global application settings and information
    /// </summary>
    public static class Application
    {
        /// <summary>
        /// Path where application executable is
        /// </summary>
        public static string RuntimePath => GetRuntimePath_Internal();
        
        /// <summary>
        /// Path to data files
        /// </summary>
        public static string DataPath => GetDataPath_Internal();
        
        /// <summary>
        /// Request to quit application in the end of this frame
        /// </summary>
        public static void Quit()
        {
            Quit_Internal();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Quit_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetRuntimePath_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetDataPath_Internal();
    }
}