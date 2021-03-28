using System.Runtime.CompilerServices;

namespace Iron
{
    public static class Random
    {
        public static float NextFloat(float min, float max) => NextFloat_Internal(min, max);

        public static int NextInt(int min, int max) => NextInt_Internal(min, max);

        public static float PerlinNoise(float x, float y) => PerlinNoise_Internal(x, y);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float NextFloat_Internal(float min, float max);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int NextInt_Internal(int min, int max);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float PerlinNoise_Internal(float x, float y);
    }
}