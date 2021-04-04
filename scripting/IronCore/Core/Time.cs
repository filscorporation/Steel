using System.Runtime.CompilerServices;

namespace Iron
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
        /// Time multiplicator
        /// </summary>
        public static float TimeScale
        {
            get => GetTimeScale_Internal();
            set => SetTimeScale_Internal(value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetDeltaTime_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetTimeScale_Internal();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float SetTimeScale_Internal(float timeScale);
    }
}