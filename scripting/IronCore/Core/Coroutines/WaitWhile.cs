using System;

namespace Iron
{
    /// <summary>
    /// Delays coroutine action while predicate is true
    /// </summary>
    public class WaitWhile
    {
        internal readonly Func<bool> Predicate;

        /// <summary>
        /// Delays coroutine action while predicate is true
        /// </summary>
        /// <param name="predicate">Predicate to check</param>
        public WaitWhile(Func<bool> predicate)
        {
            Predicate = predicate;
        }
    }
}