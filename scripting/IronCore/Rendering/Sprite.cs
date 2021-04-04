using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Loaded sprite
    /// </summary>
    public class Sprite
    {
        internal Sprite(uint id)
        {
            ID = id;
        }
        
        /// <summary>
        /// Resource unique identificator
        /// </summary>
        public uint ID { get; private set; }

        /// <summary>
        /// Sprite width in pixels
        /// </summary>
        public int Width => GetWidth_Internal(ID);
        
        /// <summary>
        /// Sprite height in pixels
        /// </summary>
        public int Height => GetHeight_Internal(ID);

        /// <summary>
        /// Sets sprite as sprite sheet, cuts it to multiple sprites
        /// </summary>
        /// <param name="tileWidth">Expected one tile width</param>
        /// <param name="tileHeight">Expected one tile height</param>
        public void SetAsSpriteSheet(int tileWidth, int tileHeight)
        {
            SetAsSpriteSheet_Internal(ID, tileWidth, tileHeight);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAsSpriteSheet_Internal(uint spriteID, int tileWidth, int tileHeight);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetWidth_Internal(uint spriteID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetHeight_Internal(uint spriteID);
    }
}