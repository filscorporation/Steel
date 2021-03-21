using System.Runtime.CompilerServices;

namespace Iron
{
    public class UIText : UIComponent
    {
        public string Text
        {
            get => GetText_Internal(Entity.ID);
            set => SetText_Internal(Entity.ID, value);
        }
        
        public int TextSize
        {
            get => GetTextSize_Internal(Entity.ID);
            set => SetTextSize_Internal(Entity.ID, value);
        }
        
        public bool IsTextAutoSize
        {
            get => GetIsTextAutoSize_Internal(Entity.ID);
            set => SetIsTextAutoSize_Internal(Entity.ID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetText_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetText_Internal(uint entityID, string text);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetTextSize_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTextSize_Internal(uint entityID, int textSize);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetIsTextAutoSize_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetIsTextAutoSize_Internal(uint entityID, bool isAutoSize);
    }
}