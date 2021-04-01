using System.Collections.Generic;

namespace Iron
{
    public class CallbackList
    {
        private readonly List<CallbackWrapper> callbackList = new List<CallbackWrapper>();
        internal bool IsActive = true;

        public void AddCallback(CallbackAction action)
        {
            callbackList.Add(new CallbackWrapper(action));
        }
        
        public void RemoveCallback(CallbackAction action)
        {
            callbackList.Remove(new CallbackWrapper(action));
        }

        public void ClearCallbacks()
        {
            callbackList.Clear();
        }

        public void Invoke()
        {
            if (!IsActive)
                return;
            
            foreach (CallbackWrapper callbackWrapper in callbackList)
            {
                callbackWrapper.Invoke();
            }
        }
    }
}