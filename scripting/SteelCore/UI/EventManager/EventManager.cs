using System.Collections.Concurrent;

namespace Steel
{
    /// <summary>
    /// Redirects engine calls to registered callbacks
    /// </summary>
    internal static class EventManager
    {
        private static readonly ConcurrentDictionary<(uint, CallbackType), CallbackList> eventCallbacks
            = new ConcurrentDictionary<(uint, CallbackType), CallbackList>();

        internal static void RegisterCallbacks(uint entityID, CallbackType callbackType, CallbackList callbackList)
        {
            if (!eventCallbacks.TryAdd((entityID, callbackType), callbackList))
            {
                Log.LogError("Trying to register multiple callback lists for one entity");
                return;
            }
        }

        internal static void DeregisterCallbacks(uint entityID, CallbackType callbackType)
        {
            eventCallbacks.TryRemove((entityID, callbackType), out _);
        }

        private static void InvokeCallbacks(uint entityID, CallbackType callbackType)
        {
            if (eventCallbacks.TryGetValue((entityID, callbackType), out CallbackList callbackList))
            {
                callbackList.Invoke();
            }
        }
    }
}