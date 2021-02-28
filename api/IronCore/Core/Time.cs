using System.Runtime.CompilerServices;

namespace Iron
{
    public static class Time
    {
        public static float DeltaTime => GetDeltaTime_Internal();

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