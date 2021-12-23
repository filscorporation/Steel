using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Interactable UI element with click callback
    /// </summary>
    public class UIButton : UIComponent
    {
        /// <summary>
        /// Callback to call when button gets clicked by mouse buttons
        /// </summary>
        public CallbackList OnClick
        {
            get
            {
                if (onClick == null)
                {
                    onClick = new CallbackList();
                    EventManager.RegisterCallbacks(Entity.EntityID, CallbackType.ButtonClick, onClick);
                }
                return onClick;
            }
        }
        private CallbackList onClick;

        /// <summary>
        /// Image, affected by button transitions
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
        /// Should button be interactable (clickable)
        /// </summary>
        public bool Interactable
        {
            get => GetInteractable_Internal(Entity.EntityID);
            set => SetInteractable_Internal(Entity.EntityID, value);
        }

        /// <summary>
        /// How button reacts to mouse interactions
        /// </summary>
        public TransitionsInfo Transition
        {
            get => GetTransition_Internal(Entity.EntityID).ToTransitionsInfo();
            set => SetTransition_Internal(Entity.EntityID, value.ToInternal());
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetTargetImage_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTargetImage_Internal(uint entityID, uint targetID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetInteractable_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetInteractable_Internal(uint entityID, bool interactable);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern TransitionsInfo_Internal GetTransition_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTransition_Internal(uint entityID, TransitionsInfo_Internal transitions);
    }
}