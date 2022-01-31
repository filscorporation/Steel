using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Layout component that arranges its children by their size
    /// </summary>
    public class UILayoutGroup : UIComponent
    {
        /// <summary>
        /// Type of layout group
        /// </summary>
        public enum LayoutType
        {
            Horizontal,
            Vertical,
        }

        /// <summary>
        /// Add new layout element
        /// </summary>
        /// <param name="minWidth">Min width element should have</param>
        /// <param name="minHeight">Min height element should have</param>
        /// <param name="prefWidth">Preferred width so more will be extra</param>
        /// <param name="prefHeight">Preferred height so more will be extra</param>
        /// <returns>Element entity</returns>
        public Entity AddElement(float minWidth, float minHeight, float prefWidth, float prefHeight)
        {
            return new Entity(AddElement_Internal(Entity.EntityID, minWidth, minHeight, prefWidth, prefHeight));
        }
        
        /// <summary>
        /// Add new layout element
        /// </summary>
        /// <param name="element">Element entity (empty UI element or with <see cref="UILayoutGroup"/> component attached)</param>
        /// <returns>Element entity</returns>
        public Entity AddElement(Entity element)
        {
            return new Entity(AddElement_Internal2(Entity.EntityID, element.EntityID));
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint AddElement_Internal(uint entityID, float minWidth, float minHeight, float prefWidth, float prefHeight);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint AddElement_Internal2(uint entityID, uint elementID);
    }
}