using System.Runtime.CompilerServices;

namespace Iron
{
    public class Input
    {
        public static bool IsKeyPressed(KeyCode code)
        {
            return IsKeyPressed_Internal((int)code);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsKeyPressed_Internal(int code);
    }
}