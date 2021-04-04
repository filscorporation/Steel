using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Methods to create UI elements and access UI layout properties
    /// </summary>
    public static class UI
    {
        /// <summary>
        /// Creates new UI element in layout
        /// </summary>
        /// <returns>Created entity</returns>
        public static Entity CreateUIElement() => new Entity(CreateUIElement_Internal());
        
        /// <summary>
        /// Creates new UI element in layout
        /// </summary>
        /// <param name="name">Entity name</param>
        /// <returns>Created entity</returns>
        public static Entity CreateUIElement(string name) => new Entity(CreateUIElement_Internal2(name));
        
        /// <summary>
        /// Creates new UI element in layout
        /// </summary>
        /// <param name="name">Entity name</param>
        /// <param name="parent">Entity's parent in the hierarchy</param>
        /// <returns>Created entity</returns>
        public static Entity CreateUIElement(string name, Entity parent)
            => new Entity(CreateUIElement_Internal3(name, parent?.ID ?? Entity.NULL_ENTITY_ID));

        /// <summary>
        /// Is there any UI object under mouse cursor
        /// </summary>
        public static bool IsPointerOverUI() => IsPointerOverUI_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIElement_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIElement_Internal2(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateUIElement_Internal3(string name, uint parentEntityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsPointerOverUI_Internal();
    }
}