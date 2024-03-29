﻿using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Loaded sprite
    /// </summary>
    public class Sprite : Resource
    {
        internal Sprite(ulong id) : base(id) { }

        /// <summary>
        /// Texture ID in GPU
        /// </summary>
        public uint TextureID => GetTextureID_Internal(ID);
        
        /// <summary>
        /// Sprite width in pixels
        /// </summary>
        public int Width => GetWidth_Internal(ID);
        
        /// <summary>
        /// Sprite height in pixels
        /// </summary>
        public int Height => GetHeight_Internal(ID);

        /// <summary>
        /// Image size in real world units
        /// </summary>
        public int PixelsPerUnit
        {
            get => GetPixelsPerUnit_Internal(ID);
            set => SetPixelsPerUnit_Internal(ID, value);
        }
        
        /// <summary>
        /// Sprite center of rotation
        /// </summary>
        public Vector2 Pivot
        {
            get
            {
                GetPivot_Internal(ID, out Vector2 pivot);
                return pivot;
            }
            set => SetPivot_Internal(ID, ref value);
        }

        /// <summary>
        /// Disable sprite sheet or 9-slicing for sprite if any was enabled
        /// </summary>
        public void SetAsNormal()
        {
            SetAsNormal_Internal(ID);
        }

        /// <summary>
        /// Sets sprite as sprite sheet, cuts it to multiple sprites
        /// </summary>
        /// <param name="tileWidth">Expected one tile width</param>
        /// <param name="tileHeight">Expected one tile height</param>
        /// <remarks>This will disable 9-slicing if it was enabled</remarks>
        public void SetAsSpriteSheet(int tileWidth, int tileHeight)
        {
            SetAsSpriteSheet_Internal(ID, tileWidth, tileHeight);
        }

        /// <summary>
        /// Slice sprite in 9 parts, which scale independently, allowing to reuse it for different sizes
        /// </summary>
        /// <param name="offset">Offset in pixels from border for slices at each side</param>
        /// <remarks>This will disable sprite sheet if it was enabled</remarks>
        public void SetAs9Sliced(int offset)
        {
            SetAs9Sliced_Internal(ID, offset);
        }

        /// <summary>
        /// Slice sprite in 9 parts, which scale independently, allowing to reuse it for different sizes
        /// </summary>
        /// <param name="offsetTop">Offset in pixels from top border for slices</param>
        /// <param name="offsetBottom">Offset in pixels from bottom border for slices</param>
        /// <param name="offsetLeft">Offset in pixels from left border for slices</param>
        /// <param name="offsetRight">Offset in pixels from right border for slices</param>
        /// <remarks>This will disable sprite sheet if it was enabled</remarks>
        public void SetAs9Sliced(int offsetTop, int offsetBottom, int offsetLeft, int offsetRight)
        {
            SetAs9Sliced_Internal2(ID, offsetTop, offsetBottom, offsetLeft, offsetRight);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetTextureID_Internal(ulong spriteID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetWidth_Internal(ulong spriteID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetHeight_Internal(ulong spriteID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetPixelsPerUnit_Internal(ulong spriteID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetPixelsPerUnit_Internal(ulong spriteID, int pixelsPerUnit);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector2 GetPivot_Internal(ulong spriteID, out Vector2 pivot);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetPivot_Internal(ulong spriteID, ref Vector2 pivot);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAsSpriteSheet_Internal(ulong spriteID, int tileWidth, int tileHeight);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAsNormal_Internal(ulong spriteID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAs9Sliced_Internal(ulong spriteID, int offset);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAs9Sliced_Internal2(ulong spriteID, int offsetTop, int offsetBottom, int offsetLeft, int offsetRight);
    }
}