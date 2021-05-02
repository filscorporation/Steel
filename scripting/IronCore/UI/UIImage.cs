using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// UI element that will get rendered in the UI layout
    /// </summary>
    public class UIImage : UIComponent
    {
        /// <summary>
        /// Sprite to render
        /// </summary>
        public Sprite Sprite
        {
            get => new Sprite(GetSprite_Internal(Entity.ID));
            set => SetSprite_Internal(Entity.ID, value?.ID ?? Resource.NULL_RESOURCE_ID);
        }

        /// <summary>
        /// Should this element consume UI events
        /// </summary>
        public bool ConsumeEvents
        {
            get => GetConsumeEvents_Internal(Entity.ID);
            set => SetConsumeEvents_Internal(Entity.ID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetSprite_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSprite_Internal(uint entityID, uint spriteID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetConsumeEvents_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetConsumeEvents_Internal(uint entityID, bool consume);
    }
}