using System.Runtime.CompilerServices;

namespace Iron
{
    public class Input
    {
        /// <summary>
        /// Position of the mouse cursor
        /// </summary>
        public static Vector2 MousePosition
        {
            get
            {
                GetMousePosition_Internal(out Vector2 position);
                return position;
            }
        }
        
        /// <summary>
        /// Mouse wheel scroll delta
        /// </summary>
        public static Vector2 MouseScrollDelta
        {
            get
            {
                GetMouseScrollDelta_Internal(out Vector2 delta);
                return delta;
            }
        }
        
        /// <summary>
        /// Is key pressed or held this frame
        /// </summary>
        /// <param name="code">Key code</param>
        /// <returns>True if key is in the pressed state</returns>
        public static bool IsKeyPressed(KeyCode code)
        {
            return IsKeyPressed_Internal((int)code);
        }
        
        /// <summary>
        /// Is key just pressed this frame
        /// </summary>
        /// <param name="code">Key code</param>
        /// <returns>True if this is the first frame key was pressed</returns>
        public static bool IsKeyJustPressed(KeyCode code)
        {
            return IsKeyJustPressed_Internal((int)code);
        }
        
        /// <summary>
        /// Is key just released this frame
        /// </summary>
        /// <param name="code">Key code</param>
        /// <returns>True if this is the first frame key got released</returns>
        public static bool IsKeyJustReleased(KeyCode code)
        {
            return IsKeyJustReleased_Internal((int)code);
        }
        
        /// <summary>
        /// Is mouse button pressed or held this frame
        /// </summary>
        /// <param name="code">Mouse button code</param>
        /// <returns>True if mouse button is in the pressed state</returns>
        public static bool IsMousePressed(MouseCodes code)
        {
            return IsMousePressed_Internal((int)code);
        }
        
        /// <summary>
        /// Is mouse button just pressed this frame
        /// </summary>
        /// <param name="code">Mouse button code</param>
        /// <returns>True if this is the first frame mouse button was pressed</returns>
        public static bool IsMouseJustPressed(MouseCodes code)
        {
            return IsMouseJustPressed_Internal((int)code);
        }
        
        /// <summary>
        /// Is mouse button just released this frame
        /// </summary>
        /// <param name="code">Mouse button code</param>
        /// <returns>True if this is the first frame mouse button was released</returns>
        public static bool IsMouseJustReleased(MouseCodes code)
        {
            return IsMouseJustReleased_Internal((int)code);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetMousePosition_Internal(out Vector2 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetMouseScrollDelta_Internal(out Vector2 delta);

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