using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Application screen properties
    /// </summary>
    public static class Screen
    {
        /// <summary>
        /// Window width in pixels
        /// </summary>
        public static int Width
        {
            get => GetWidth_Internal();
            set => SetWidth_Internal(value);
        }
        
        /// <summary>
        /// Window height in pixels
        /// </summary>
        public static int Height
        {
            get => GetHeight_Internal();
            set => SetHeight_Internal(value);
        }
        
        /// <summary>
        /// Is window in fullscreen mode
        /// </summary>
        public static bool Fullscreen
        {
            get => GetFullscreen_Internal();
            set => SetFullscreen_Internal(value);
        }

        /// <summary>
        /// Is screen minimized
        /// </summary>
        public static bool IsMinimized => GetIsMinimized_Internal();

        /// <summary>
        /// Screen culling color
        /// </summary>
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
        private static extern bool GetIsMinimized_Internal();
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Color GetColor_Internal();
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetColor_Internal(Color color);
    }
}