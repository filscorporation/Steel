using System.Runtime.InteropServices;

namespace Iron
{
    public struct RayCastHit
    {
        public Entity Entity;
        public Vector2 HitPoint;
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