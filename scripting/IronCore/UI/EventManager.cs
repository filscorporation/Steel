using System.Collections.Concurrent;

namespace Iron
{
    /// <summary>
    /// Redirects engine calls to registered callbacks
    /// </summary>
    internal static class EventManager
    {
        private static readonly ConcurrentDictionary<ulong, CallbackList> eventCallbacks = new ConcurrentDictionary<ulong, CallbackList>();

        internal static void RegisterCallbacks(uint entityID, CallbackList callbackList)
        {
            if (!eventCallbacks.TryAdd(entityID, callbackList))
            {
                Log.LogError("Trying to register multiple callback lists for one entity");
                return;
            }
        }

        internal static void DeregisterCallbacks(uint entityID)
        {
            eventCallbacks.TryRemove(entityID, out _);
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