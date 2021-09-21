using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Steel
{
    /// <summary>
    /// Layout component that arranges its children by their size
    /// </summary>
    public class UILayoutGroup : UIComponent
    {
        /// <summary>
        /// Layout element info
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct ElementInfo
        {
            /// <summary>
            /// Min width element should have
            /// </summary>
            public float MinWidth;
            
            /// <summary>
            /// Min height element should have
            /// </summary>
            public float MinHeight;
            
            /// <summary>
            /// Preferred width so more will be extra
            /// </summary>
            public float PreferredWidth;
            
            /// <summary>
            /// Preferred height so more will be extra
            /// </summary>
            public float PreferredHeight;
        }

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
        /// <param name="info">Data to initialize new element</param>
        /// <returns>Element entity</returns>
        public Entity AddElement(ElementInfo info)
        {
            return new Entity(AddElement_Internal(Entity.ID, info));
        }
        
        /// <summary>
        /// Add new layout element
        /// </summary>
        /// <param name="element">Element entity (with <see cref="UILayoutGroup"/> component attached, or element component will be added)</param>
        /// <returns>Element entity</returns>
        public Entity AddElement(Entity element)
        {
            return new Entity(AddElement_Internal2(Entity.ID, element.ID));
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint AddElement_Internal(uint entityID, ElementInfo info);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint AddElement_Internal2(uint entityID, uint elementID);
    }
}