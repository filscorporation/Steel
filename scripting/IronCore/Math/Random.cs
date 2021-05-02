using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Random numbers generation functions
    /// </summary>
    public static class Random
    {
        /// <summary>
        /// Returns random float
        /// </summary>
        /// <param name="min">Minimum inclusive</param>
        /// <param name="max">Maximum exclusive</param>
        /// <returns>Random float between min and max</returns>
        public static float NextFloat(float min, float max) => NextFloat_Internal(min, max);

        /// <summary>
        /// Returns random integer
        /// </summary>
        /// <param name="min">Minimum inclusive</param>
        /// <param name="max">Maximum inclusive</param>
        /// <returns>Random integer between min and max</returns>
        public static int NextInt(int min, int max) => NextInt_Internal(min, max);

        /// <summary>
        /// Returns random float smoother across x and y
        /// </summary>
        /// <returns>Value between -1 and 1</returns>
        public static float PerlinNoise(float x, float y) => PerlinNoise_Internal(x, y);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float NextFloat_Internal(float min, float max);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int NextInt_Internal(int min, int max);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float PerlinNoise_Internal(float x, float y);
    }
}