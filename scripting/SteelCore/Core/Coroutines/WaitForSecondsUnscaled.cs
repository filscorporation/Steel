namespace Steel
{
    /// <summary>
    /// Delays coroutine action by number of seconds not affected by <see cref="Time.TimeScale"/>
    /// </summary>
    public class WaitForSecondsUnscaled
    {
        internal float Duration;
        
        /// <summary>
        /// Delays coroutine action by number of seconds not affected by <see cref="Time.TimeScale"/>
        /// </summary>
        /// <param name="duration">Delay in seconds</param>
        public WaitForSecondsUnscaled(float duration)
        {
            Duration = duration;
        }
    }
}