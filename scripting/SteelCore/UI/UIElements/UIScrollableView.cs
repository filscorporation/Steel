using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// UI view with scrollable content
    /// </summary>
    public class UIScrollableView : UIComponent
    {
        /// <summary>
        /// Content entity that wil get scrolled (add child elements and set it's size)
        /// </summary>
        public Entity Content => new Entity(GetContent_Internal(Entity.EntityID));

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetContent_Internal(uint entityID);
    }
}