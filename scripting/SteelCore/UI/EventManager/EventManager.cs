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

        internal static CallbackList TryAddCallbacks(uint entityID, CallbackType callbackType)
        {
            if (eventCallbacks.TryGetValue((entityID, callbackType), out CallbackList callbackList))
                return callbackList;

            callbackList = new CallbackList();
            eventCallbacks.TryAdd((entityID, callbackType), callbackList);

            return callbackList;
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