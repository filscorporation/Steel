using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Time related methods and properties
    /// </summary>
    public static class Time
    {
        /// <summary>
        /// Time passed from the last frame
        /// </summary>
        public static float DeltaTime => GetDeltaTime_Internal();
        
        /// <summary>
        /// Time passed from the last frame (not affected by time scale)
        /// </summary>
        public static float UnscaledDeltaTime => GetUnscaledDeltaTime_Internal();
        
        /// <summary>
        /// Time passed since application startup
        /// </summary>
        public static float TimeSinceStartup => GetTimeSinceStartup_Internal();
        
        /// <summary>
        /// Total frames count since application startup
        /// </summary>
        public static ulong FrameCount => GetFrameCount_Internal();

        /// <summary>
        /// Fixed delta time (used by physics)
        /// </summary>
        public static float FixedDeltaTime
        {
            get => GetFixedDeltaTime_Internal();
            set => SetFixedDeltaTime_Internal(value);
        }

        /// <summary>
        /// Time scale factor (does not affect UI)
        /// </summary>
        public static float TimeScale
        {
            get => GetTimeScale_Internal();
            set => SetTimeScale_Internal(value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetDeltaTime_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetUnscaledDeltaTime_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetTimeSinceStartup_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong GetFrameCount_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetTimeScale_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float SetTimeScale_Internal(float timeScale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetFixedDeltaTime_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float SetFixedDeltaTime_Internal(float fixedDeltaTime);
    }
}