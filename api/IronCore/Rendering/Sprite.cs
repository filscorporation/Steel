using System.Runtime.CompilerServices;

namespace Iron
{
    public class Sprite
    {
        internal Sprite(ulong id)
        {
            ID = id;
        }
        
        public ulong ID { get; private set; }

        public int Width => GetWidth_Internal(ID);
        public int Height => GetHeight_Internal(ID);

        public void SetAsSpriteSheet(int tileWidth, int tileHeight)
        {
            SetAsSpriteSheet_Internal(ID, tileWidth, tileHeight);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAsSpriteSheet_Internal(ulong spriteID, int tileWidth, int tileHeight);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetWidth_Internal(ulong spriteID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetHeight_Internal(ulong spriteID);
    }
}