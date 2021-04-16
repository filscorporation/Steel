using System;
using System.Collections;
using System.Collections.Generic;

namespace Iron
{
    /// <summary>
    /// Represents action that can be delayed
    /// </summary>
    public class Coroutine
    {
        internal readonly uint Owner;
        internal readonly IEnumerator Enumerator;
        internal LinkedListNode<Coroutine> Node;

        internal bool IsDestroyed = false;
        
        internal Coroutine(uint owner, IEnumerator enumerator)
        {
            Owner = owner;
            Enumerator = enumerator;
            Node = null;
        }

        /// <summary>
        /// Coroutine from delayed action
        /// </summary>
        /// <param name="action">Action to call in coroutine</param>
        /// <param name="delay">Delay in seconds to wait</param>
        /// <returns>IEnumerator to pass in StartCoroutine</returns>
        public static IEnumerator WaitForSeconds(Action action, float delay)
        {
            yield return new WaitForSeconds(delay);
            
            action?.Invoke();
        }
        
        /// <summary>
        /// Coroutine that delays action while predicate is true
        /// </summary>
        /// <param name="action">Action to call in coroutine</param>
        /// <param name="predicate">Predicate to check</param>
        /// <returns>IEnumerator to pass in StartCoroutine</returns>
        public static IEnumerator WaitWhile(Action action, Func<bool> predicate)
        {
            yield return new WaitWhile(predicate);
            
            action?.Invoke();
        }
    }
}