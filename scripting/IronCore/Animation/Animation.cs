using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Loaded animation clip
    /// </summary>
    public class Animation
    {
        internal Animation(uint id)
        {
            ID = id;
        }
        
        /// <summary>
        /// Resource unique identificator
        /// </summary>
        public uint ID { get; private set; }

        /// <summary>
        /// Is animation looped
        /// </summary>
        public bool Loop
        {
            get => GetLoop_Internal(ID);
            set => SetLoop_Internal(ID, value);
        }

        /// <summary>
        /// Animation length in seconds
        /// </summary>
        public float Length => GetLength_Internal(ID);

        /// <summary>
        /// Creates animation from sprite sheet
        /// </summary>
        /// <param name="sprite">Sprite that was set as sprite sheet</param>
        /// <param name="length">Desired animation length</param>
        /// <returns>New animation or null if creation was not successful</returns>
        public static Animation FromSpriteSheet(Sprite sprite, float length)
        {
            uint animationID = FromSpriteSheet_Internal(sprite.ID, length);
            
            return animationID == 0 ? null : new Animation(animationID);
        }

        /// <summary>
        /// Creates animation from multiple sprites
        /// </summary>
        /// <param name="sprites">List of sprites</param>
        /// <param name="length">Desired animation length</param>
        /// <returns>New animation or null if creation was not successful</returns>
        /// <remarks>Sprites will be put evenly on the timeline</remarks>
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