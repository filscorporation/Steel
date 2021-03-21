using System.Runtime.CompilerServices;

namespace Iron
{
    public static class UI
    {
        public static Entity CreateUIElement() => new Entity(CreateUIElement_Internal());
        
        public static Entity CreateUIElement(string name) => new Entity(CreateUIElement_Internal2(name));
        
        public static Entity CreateUIElement(string name, Entity parent)
            => new Entity(CreateUIElement_Internal3(name, parent?.ID ?? Entity.NULL_ENTITY_ID));

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