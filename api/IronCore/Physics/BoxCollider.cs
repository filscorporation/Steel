using System.Runtime.CompilerServices;

namespace Iron
{
    public class BoxCollider : Collider
    {
        public Vector2 Size
        {
            get
            {
                GetSize_Internal(Entity.ID, out Vector2 size);
                return size;
            }
            set => SetSize_Internal(Entity.ID, ref value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetSize_Internal(uint entityID, out Vector2 size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSize_Internal(uint entityID, ref Vector2 size);
    }
}