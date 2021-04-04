using System.Runtime.InteropServices;

namespace Iron
{
    /// <summary>
    /// Information about line hitting physics object
    /// </summary>
    public struct RayCastHit
    {
        /// <summary>
        /// Entity that was hit
        /// </summary>
        public Entity Entity;
        
        /// <summary>
        /// Point in the world where line entered entity collider
        /// </summary>
        public Vector2 HitPoint;
        
        /// <summary>
        /// Normal vector to hit point
        /// </summary>
        public Vector2 HitNormal;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct RayCastHit_Internal
    {
        public uint EntityID;
        public Vector2 HitPoint;
        public Vector2 HitNormal;

        internal RayCastHit ToRayCastHit()
        {
            return new RayCastHit
            {
                Entity = new Entity(EntityID),
                HitPoint = HitPoint,
                HitNormal = HitNormal,
            };
        }
    }
}