using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// UI window with tabs
    /// </summary>
    public class UITabs : UIComponent
    {
        /// <summary>
        /// Tabs count
        /// </summary>
        public int TabsCount => TabsCount_Internal(Entity.EntityID);

        /// <summary>
        /// Current active tab
        /// </summary>
        public int ActiveTab
        {
            get => GetActiveTab_Internal(Entity.EntityID);
            set => SetActiveTab_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Returns tab content entity by index
        /// </summary>
        public Entity GetTab(int index)
        {
            return new Entity(GetTab_Internal(Entity.EntityID, index));
        }
        
        /// <summary>
        /// Adds new tab to this element
        /// </summary>
        public Entity AddTab(string name)
        {
            return new Entity(AddTab_Internal(Entity.EntityID, name));
        }
        
        /// <summary>
        /// Removes tab by index
        /// </summary>
        public bool RemoveTab(int index)
        {
            return RemoveTab_Internal(Entity.EntityID, index);
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