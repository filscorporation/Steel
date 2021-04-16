namespace Iron
{
    /// <summary>
    /// Delays coroutine action by number of seconds
    /// </summary>
    public class WaitForSeconds
    {
        internal float Duration;
        
        /// <summary>
        /// Delays coroutine action by number of seconds
        /// </summary>
        /// <param name="duration">Delay in seconds</param>
        public WaitForSeconds(float duration)
        {
            Duration = duration;
        }
    }
}