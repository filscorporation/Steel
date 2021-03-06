using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace Iron
{
    public class Animation
    {
        internal Animation(ulong id)
        {
            ID = id;
        }
        
        public ulong ID { get; private set; }

        public bool Loop
        {
            get => GetLoop_Internal(ID);
            set => SetLoop_Internal(ID, value);
        }

        public float Length => GetLength_Internal(ID);

        public static Animation FromSpriteSheet(Sprite sprite, float length)
        {
            ulong animationID = FromSpriteSheet_Internal(sprite.ID, length);
            
            return animationID == 0 ? null : new Animation(animationID);
        }

        public static Animation FromSprites(List<Sprite> sprites, float length)
        {
            // TODO: pass list/array
            throw new NotImplementedException();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong FromSpriteSheet_Internal(ulong spriteID, float length);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetLoop_Internal(ulong animationID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLoop_Internal(ulong animationID, bool loop);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetLength_Internal(ulong animationID);
    }
}