using System.Runtime.CompilerServices;

namespace Iron
{
    public class SpriteRenderer : Component
    {
        public Sprite Sprite
        {
            get => new Sprite(GetSprite_Internal(Entity.ID));
            set => SetSprite_Internal(Entity.ID, value?.ID ?? 0);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong GetSprite_Internal(ulong entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSprite_Internal(ulong entityID, ulong spriteID);
    }
}