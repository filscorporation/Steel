using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Box shaped physics collider
    /// </summary>
    public class BoxCollider : Collider
    {
        /// <summary>
        /// Collider size
        /// </summary>
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