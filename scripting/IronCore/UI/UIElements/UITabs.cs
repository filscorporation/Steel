using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// UI window with tabs
    /// </summary>
    public class UITabs : UIComponent
    {
        /// <summary>
        /// Tabs count
        /// </summary>
        public int TabsCount => TabsCount_Internal(Entity.ID);

        /// <summary>
        /// Current active tab
        /// </summary>
        public int ActiveTab
        {
            get => GetActiveTab_Internal(Entity.ID);
            set => SetActiveTab_Internal(Entity.ID, value);
        }
        
        /// <summary>
        /// Returns tab content entity by index
        /// </summary>
        public Entity GetTab(int index)
        {
            return new Entity(GetTab_Internal(Entity.ID, index));
        }
        
        /// <summary>
        /// Adds new tab to this element
        /// </summary>
        public Entity AddTab(string name)
        {
            return new Entity(AddTab_Internal(Entity.ID, name));
        }
        
        /// <summary>
        /// Removes tab by index
        /// </summary>
        public bool RemoveTab(int index)
        {
            return RemoveTab_Internal(Entity.ID, index);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int TabsCount_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int GetActiveTab_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetActiveTab_Internal(uint entityID, int value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint GetTab_Internal(uint entityID, int index);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint AddTab_Internal(uint entityID, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool RemoveTab_Internal(uint entityID, int index);
    }
}