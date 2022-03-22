using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Interactable UI element that displays boolean value
    /// </summary>
    public class UICheckBox : UIComponent
    {
        /// <summary>
        /// Callback to call when value is changed
        /// </summary>
        public CallbackList OnValueChanged => EventManager.TryAddCallbacks(Entity.EntityID, CallbackType.CheckBoxChangeValue);

        /// <summary>
        /// Current value (on/off)
        /// </summary>
        public bool Value
        {
            get => GetValue_Internal(Entity.EntityID);
            set => SetValue_Internal(Entity.EntityID, value);
        }

        /// <summary>
        /// Should check box be interactable (clickable)
        /// </summary>
        public bool Interactable
        {
            get => GetInteractable_Internal(Entity.EntityID);
            set => SetInteractable_Internal(Entity.EntityID, value);
        }

        /// <summary>
        /// How check box reacts to mouse interactions
        /// </summary>
        public TransitionsInfo Transition
        {
            get => GetTransition_Internal(Entity.EntityID).ToTransitionsInfo();
            set => SetTransition_Internal(Entity.EntityID, value.ToInternal());
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