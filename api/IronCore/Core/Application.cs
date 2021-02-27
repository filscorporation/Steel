using System.Runtime.CompilerServices;

namespace Iron
{
    public static class Application
    {
        public static void Quit()
        {
            Quit_Internal();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Quit_Internal();
    }
}