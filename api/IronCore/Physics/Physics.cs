using System.Runtime.CompilerServices;

namespace Iron
{
    public static class Physics
    {
        public static void Simulate(float deltaTime) => Simulate_Internal(deltaTime);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Simulate_Internal(float deltaTime);
    }
}