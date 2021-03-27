using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Iron
{
    public static class Physics
    {
        public static void Simulate(float deltaTime) => Simulate_Internal(deltaTime);

        public static IEnumerable<Entity> PointCast(Vector2 point)
        {
            return PointCast_Internal(ref point).Select(entityID => new Entity(entityID));
        }

        public static IEnumerable<Entity> AABBCast(Vector2 min, Vector2 max)
        {
            return AABBCast_Internal(ref min, ref max).Select(entityID => new Entity(entityID));
        }

        public static IEnumerable<RayCastHit> LineCast(Vector2 pointA, Vector2 pointB)
        {
            return LineCast_Internal(ref pointA, ref pointB).Select(hit => hit.ToRayCastHit());
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Simulate_Internal(float deltaTime);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint[] PointCast_Internal(ref Vector2 point);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint[] AABBCast_Internal(ref Vector2 center, ref Vector2 size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern RayCastHit_Internal[] LineCast_Internal(ref Vector2 pointA, ref Vector2 pointB);
    }
}