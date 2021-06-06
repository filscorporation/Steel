using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Interactable input field
    /// </summary>
    public class UIInputField : UIComponent
    {
        /// <summary>
        /// Callback to call when value in the input field get changed
        /// </summary>
        public CallbackList OnValueChanged
        {
            get
            {
                if (onValueChanged == null)
                {
                    onValueChanged = new CallbackList();
                    EventManager.RegisterCallbacks(Entity.ID, CallbackType.InputFieldChangeValue, onValueChanged);
                }
                return onValueChanged;
            }
        }
        private CallbackList onValueChanged;
        
        /// <summary>
        /// Callback to call when input field editing finished
        /// </summary>
        public CallbackList OnEndEditing
        {
            get
            {
                if (onEndEditing == null)
                {
                    onEndEditing = new CallbackList();
                    EventManager.RegisterCallbacks(Entity.ID, CallbackType.InputFieldEndEdit, onEndEditing);
                }
                return onEndEditing;
            }
        }
        private CallbackList onEndEditing;
        
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

        /// <summary>
        /// Should field be interactable (clickable)
        /// </summary>
        public bool Interactable
        {
            get => GetInteractable_Internal(Entity.ID);
            set => SetInteractable_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Cursor width in pixels
        /// </summary>
        public uint CursorWidth
        {
            get => GetCursorWidth_Internal(Entity.ID);
            set => SetCursorWidth_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Cursor color if it is not auto
        /// </summary>
        public Color CursorColor
        {
            get => GetCursorColor_Internal(Entity.ID);
            set => SetCursorColor_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Should cursor be same as text color
        /// </summary>
        public bool CursorAutoColor
        {
            get => GetCursorAutoColor_Internal(Entity.ID);
            set => SetCursorAutoColor_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Can field support multiline text
        /// </summary>
        public bool IsMultiline
        {
            get => GetIsMultiline_Internal(Entity.ID);
            set => SetIsMultiline_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Text type field can support
        /// </summary>
        public TextType TextType
        {
            get => GetTextType_Internal(Entity.ID);
            set => SetTextType_Internal(Entity.ID, value);
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
    }
}