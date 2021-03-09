using System.Runtime.CompilerServices;

namespace Iron
{
    public class Sprite
    {
        internal Sprite(uint id)
        {
            ID = id;
        }
        
        public uint ID { get; private set; }

        public int Width => GetWidth_Internal(ID);
        public int Height => GetHeight_Internal(ID);

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