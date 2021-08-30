using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Interactable UI element that displays boolean value
    /// </summary>
    public class UICheckBox : UIComponent
    {
        /// <summary>
        /// Callback to call when value is changed
        /// </summary>
        public CallbackList OnValueChanged
        {
            get
            {
                if (onValueChanged == null)
                {
                    onValueChanged = new CallbackList();
                    EventManager.RegisterCallbacks(Entity.ID, CallbackType.CheckBoxChangeValue, onValueChanged);
                }
                return onValueChanged;
            }
        }
        private CallbackList onValueChanged;

        /// <summary>
        /// Current value (on/off)
        /// </summary>
        public bool Value
        {
            get => GetValue_Internal(Entity.ID);
            set => SetValue_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Should check box be interactable (clickable)
        /// </summary>
        public bool Interactable
        {
            get => GetInteractable_Internal(Entity.ID);
            set => SetInteractable_Internal(Entity.ID, value);
        }

        /// <summary>
        /// How check box reacts to mouse interactions
        /// </summary>
        public TransitionsInfo Transition
        {
            get => GetTransition_Internal(Entity.ID).ToTransitionsInfo();
            set => SetTransition_Internal(Entity.ID, value.ToInternal());
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetValue_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetValue_Internal(uint entityID, bool value);

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