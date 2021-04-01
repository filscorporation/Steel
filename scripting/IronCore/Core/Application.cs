using System.Runtime.CompilerServices;

namespace Iron
{
    public static class Application
    {
        public static ApplicationState State => GetState_Internal();

        public static string RuntimePath => GetRuntimePath_Internal();
        
        public static string DataPath => GetDataPath_Internal();
        
        public static void Quit()
        {
            Quit_Internal();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Quit_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ApplicationState GetState_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetRuntimePath_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetDataPath_Internal();
    }
}