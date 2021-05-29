using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Interactable input field
    /// </summary>
    public class UIInputField : UIComponent
    {
        /// <summary>
        /// Text component, controlled by input field
        /// </summary>
        public UIText TargetText
        {
            get
            {
                uint targetID = GetTargetText_Internal(Entity.ID);
                if (targetID == Entity.NULL_ENTITY_ID)
                    return null;
                
                UIText text = new UIText();
                text.Entity = new Entity(targetID);
                
                return text;
            }
            set => SetTargetText_Internal(Entity.ID, value?.Entity.ID ?? Entity.NULL_ENTITY_ID);
        }
        
        /// <summary>
        /// Image, affected by field transitions
        /// </summary>
        public UIImage TargetImage
        {
            get
            {
                uint targetID = GetTargetImage_Internal(Entity.ID);
                if (targetID == Entity.NULL_ENTITY_ID)
                    return null;
                
                UIImage image = new UIImage();
                image.Entity = new Entity(targetID);
                
                return image;
            }
            set => SetTargetImage_Internal(Entity.ID, value?.Entity.ID ?? Entity.NULL_ENTITY_ID);
        }
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetTargetText_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTargetText_Internal(uint entityID, uint targetID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetTargetImage_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTargetImage_Internal(uint entityID, uint targetID);
    }
}