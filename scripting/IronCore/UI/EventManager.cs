using System.Collections.Generic;

namespace Iron
{
    /// <summary>
    /// Redirects engine calls to registered callbacks
    /// </summary>
    internal static class EventManager
    {
        private static readonly Dictionary<uint, CallbackList> eventCallbacks = new Dictionary<uint, CallbackList>();

        internal static void RegisterCallbacks(uint entityID, CallbackList callbackList)
        {
            if (eventCallbacks.ContainsKey(entityID))
            {
                Log.LogError("Trying to register multiple callback lists for one entity");
                return;
            }

            eventCallbacks[entityID] = callbackList;
        }

        internal static void DeregisterCallbacks(uint entityID)
        {
            if (!eventCallbacks.ContainsKey(entityID))
            {
                Log.LogError("Trying to deregister non existing callback list");
                return;
            }

            eventCallbacks.Remove(entityID);
        }

        private static void InvokeCallbacks(uint entityID)
        {
            if (eventCallbacks.TryGetValue(entityID, out CallbackList callbackList))
            {
                callbackList.Invoke();
            }
        }
    }
}