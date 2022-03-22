using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Interactable input field
    /// </summary>
    public class UIInputField : UIComponent
    {
        /// <summary>
        /// Callback to call when value in the input field get changed
        /// </summary>
        public CallbackList OnValueChanged => EventManager.TryAddCallbacks(Entity.EntityID, CallbackType.InputFieldChangeValue);
        
        /// <summary>
        /// Callback to call when input field editing finished
        /// </summary>
        public CallbackList OnEndEditing => EventManager.TryAddCallbacks(Entity.EntityID, CallbackType.InputFieldEndEdit);
        
        /// <summary>
        /// Text component, controlled by input field
        /// </summary>
        public UIText TargetText
        {
            get
            {
                uint targetID = GetTargetText_Internal(Entity.EntityID);
                if (targetID == Entity.NULL_ENTITY_ID)
                    return null;
                
                UIText text = new UIText();
                text.Entity = new Entity(targetID);
                
                return text;
            }
            set => SetTargetText_Internal(Entity.EntityID, value?.Entity.EntityID ?? Entity.NULL_ENTITY_ID);
        }
        
        /// <summary>
        /// Image, affected by field transitions
        /// </summary>
        public UIImage TargetImage
        {
            get
            {
                uint targetID = GetTargetImage_Internal(Entity.EntityID);
                if (targetID == Entity.NULL_ENTITY_ID)
                    return null;
                
                UIImage image = new UIImage();
                image.Entity = new Entity(targetID);
                
                return image;
            }
            set => SetTargetImage_Internal(Entity.EntityID, value?.Entity.EntityID ?? Entity.NULL_ENTITY_ID);
        }

        /// <summary>
        /// Should field be interactable (clickable)
        /// </summary>
        public bool Interactable
        {
            get => GetInteractable_Internal(Entity.EntityID);
            set => SetInteractable_Internal(Entity.EntityID, value);
        }

        /// <summary>
        /// Cursor width in pixels
        /// </summary>
        public uint CursorWidth
        {
            get => GetCursorWidth_Internal(Entity.EntityID);
            set => SetCursorWidth_Internal(Entity.EntityID, value);
        }

        /// <summary>
        /// Cursor color if it is not auto
        /// </summary>
        public Color CursorColor
        {
            get => GetCursorColor_Internal(Entity.EntityID);
            set => SetCursorColor_Internal(Entity.EntityID, value);
        }

        /// <summary>
        /// Should cursor be same as text color
        /// </summary>
        public bool CursorAutoColor
        {
            get => GetCursorAutoColor_Internal(Entity.EntityID);
            set => SetCursorAutoColor_Internal(Entity.EntityID, value);
        }

        /// <summary>
        /// Can field support multiline text
        /// </summary>
        public bool IsMultiline
        {
            get => GetIsMultiline_Internal(Entity.EntityID);
            set => SetIsMultiline_Internal(Entity.EntityID, value);
        }

        /// <summary>
        /// Text type field can support
        /// </summary>
        public TextType TextType
        {
            get => GetTextType_Internal(Entity.EntityID);
            set => SetTextType_Internal(Entity.EntityID, value);
        }

        /// <summary>
        /// Selection color
        /// </summary>
        public Color SelectionColor
        {
            get => GetSelectionColor_Internal(Entity.EntityID);
            set => SetSelectionColor_Internal(Entity.EntityID, value);
        }
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetTargetText_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTargetText_Internal(uint entityID, uint targetID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetTargetImage_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTargetImage_Internal(uint entityID, uint targetID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetInteractable_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetInteractable_Internal(uint entityID, bool interactable);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetCursorWidth_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCursorWidth_Internal(uint entityID, uint width);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Color GetCursorColor_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCursorColor_Internal(uint entityID, Color color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetCursorAutoColor_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCursorAutoColor_Internal(uint entityID, bool isAuto);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetIsMultiline_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetIsMultiline_Internal(uint entityID, bool isMultiline);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern TextType GetTextType_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTextType_Internal(uint entityID, TextType type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Color GetSelectionColor_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSelectionColor_Internal(uint entityID, Color color);
    }
}