using System.Runtime.CompilerServices;

namespace Iron
{
    public class UIButton : UIComponent
    {
        ~UIButton()
        {
            if (OnClick != null)
            {
                OnClick.IsActive = false;
                EventManager.DeregisterCallbacks(Entity.ID);
            }
        }

        private CallbackList onClick;
        public CallbackList OnClick
        {
            get
            {
                if (onClick == null)
                {
                    onClick = new CallbackList();
                    EventManager.RegisterCallbacks(Entity.ID, OnClick);
                }
                return onClick;
            }
        }

        public Sprite Sprite
        {
            get => new Sprite(GetSprite_Internal(Entity.ID));
            set => SetSprite_Internal(Entity.ID, value?.ID ?? 0);
        }

        public bool Interactable
        {
            get => GetInteractable_Internal(Entity.ID);
            set => SetInteractable_Internal(Entity.ID, value);
        }

        public ButtonTransitionsInfo Transition
        {
            get => GetTransition_Internal(Entity.ID).ToButtonTransitionsInfo();
            set => SetTransition_Internal(Entity.ID, value.ToInternal());
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetSprite_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSprite_Internal(uint entityID, uint spriteID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetInteractable_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetInteractable_Internal(uint entityID, bool interactable);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ButtonTransitionsInfo_Internal GetTransition_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTransition_Internal(uint entityID, ButtonTransitionsInfo_Internal transitions);
    }
}