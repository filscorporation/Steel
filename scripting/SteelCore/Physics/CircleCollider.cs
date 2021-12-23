using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Circle shaped physics collider
    /// </summary>
    public class CircleCollider : Collider
    {
        /// <summary>
        /// Collider radius
        /// </summary>
        public float Radius
        {
            get => GetRadius_Internal(Entity.EntityID);
            set => SetRadius_Internal(Entity.EntityID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetRadius_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetRadius_Internal(uint entityID, float radius);
    }
}