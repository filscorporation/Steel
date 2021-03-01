using System.Runtime.CompilerServices;

namespace Iron
{
    public class Input
    {
        public static Vector2 MousePosition => GetMousePosition_Internal();
        public static Vector2 MouseScrollDelta => GetMouseScrollDelta_Internal();
        
        public static bool IsKeyPressed(KeyCode code)
        {
            return IsKeyPressed_Internal((int)code);
        }
        
        public static bool IsKeyJustPressed(KeyCode code)
        {
            return IsKeyJustPressed_Internal((int)code);
        }
        
        public static bool IsKeyJustReleased(KeyCode code)
        {
            return IsKeyJustReleased_Internal((int)code);
        }
        
        public static bool IsMousePressed(MouseCodes code)
        {
            return IsMousePressed_Internal((int)code);
        }
        
        public static bool IsMouseJustPressed(MouseCodes code)
        {
            return IsMouseJustPressed_Internal((int)code);
        }
        
        public static bool IsMouseJustReleased(MouseCodes code)
        {
            return IsMouseJustReleased_Internal((int)code);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector2 GetMousePosition_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector2 GetMouseScrollDelta_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsKeyPressed_Internal(int code);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsKeyJustPressed_Internal(int code);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsKeyJustReleased_Internal(int code);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsMousePressed_Internal(int code);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsMouseJustPressed_Internal(int code);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsMouseJustReleased_Internal(int code);
    }
}