using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace Iron
{
    public class Animation
    {
        internal Animation(uint id)
        {
            ID = id;
        }
        
        public uint ID { get; private set; }

        public bool Loop
        {
            get => GetLoop_Internal(ID);
            set => SetLoop_Internal(ID, value);
        }

        public float Length => GetLength_Internal(ID);

        public static Animation FromSpriteSheet(Sprite sprite, float length)
        {
            uint animationID = FromSpriteSheet_Internal(sprite.ID, length);
            
            return animationID == 0 ? null : new Animation(animationID);
        }

        public static Animation FromSprites(List<Sprite> sprites, float length)
        {
            // TODO: pass list/array
            throw new NotImplementedException();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint FromSpriteSheet_Internal(uint spriteID, float length);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetLoop_Internal(uint animationID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLoop_Internal(uint animationID, bool loop);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetLength_Internal(uint animationID);
    }
}