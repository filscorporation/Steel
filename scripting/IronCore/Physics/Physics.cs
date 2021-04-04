using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Physics world methods, properties and queries
    /// </summary>
    public static class Physics
    {
        /// <summary>
        /// Simulates physics world for requested time step
        /// </summary>
        /// <param name="deltaTime">Time step to simulate</param>
        public static void Simulate(float deltaTime) => Simulate_Internal(deltaTime);

        /// <summary>
        /// Returns all entities overlapped by the point
        /// </summary>
        /// <param name="point">Point in the scene</param>
        /// <returns>All entities overlapped by the point</returns>
        public static IEnumerable<Entity> PointCast(Vector2 point)
        {
            return PointCast_Internal(ref point).Select(entityID => new Entity(entityID));
        }

        /// <summary>
        /// Returns all entities with AABB of their colliders to be overlapped by requested AABB
        /// </summary>
        /// <param name="center">Requested AABB center</param>
        /// <param name="size">Requested AABB size</param>
        /// <returns>All entities overlapped by AABB</returns>
        /// <remarks>Because of how AABB works, results can be imprecise</remarks>
        public static IEnumerable<Entity> AABBCast(Vector2 center, Vector2 size)
        {
            return AABBCast_Internal(ref center, ref size).Select(entityID => new Entity(entityID));
        }

        /// <summary>
        /// Casts a line between two points and returns all entities with colliders hit by this line
        /// </summary>
        /// <param name="pointA">Starting line point</param>
        /// <param name="pointB">Ending line point</param>
        /// <returns>All entities overlapped by the line</returns>
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