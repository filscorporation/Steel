using System.Runtime.CompilerServices;

namespace Iron
{
    public static class Screen
    {
        public static int Width
        {
            get => GetWidth_Internal();
            set => SetWidth_Internal(value);
        }
        
        public static int Height
        {
            get => GetHeight_Internal();
            set => SetHeight_Internal(value);
        }
        
        public static bool Fullscreen
        {
            get => GetFullscreen_Internal();
            set => SetFullscreen_Internal(value);
        }
        
        public static Color Color
        {
            get => GetColor_Internal();
            set => SetColor_Internal(value);
        }
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetWidth_Internal();
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetWidth_Internal(int width);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetHeight_Internal();
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetHeight_Internal(int height);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetFullscreen_Internal();
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetFullscreen_Internal(bool fullscreen);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Color GetColor_Internal();
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetColor_Internal(Color color);
    }
}