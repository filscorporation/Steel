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

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetSprite_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSprite_Internal(uint entityID, uint spriteID);
    }
}