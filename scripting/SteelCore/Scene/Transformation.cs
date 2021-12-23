using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Component representing entity position, rotation and scale in the scene
    /// </summary>
    public class Transformation : Component
    {
        /// <summary>
        /// Global position
        /// </summary>
        public Vector3 Position
        {
            get => GetPosition_Internal(Entity.EntityID);
            set => SetPosition_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Local position
        /// </summary>
        public Vector3 LocalPosition
        {
            get => GetLocalPosition_Internal(Entity.EntityID);
            set => SetLocalPosition_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Global rotation
        /// </summary>
        public Vector3 Rotation
        {
            get => GetRotation_Internal(Entity.EntityID);
            set => SetRotation_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Local rotation
        /// </summary>
        public Vector3 LocalRotation
        {
            get => GetLocalRotation_Internal(Entity.EntityID);
            set => SetLocalRotation_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Global scale
        /// </summary>
        public Vector3 Scale
        {
            get => GetScale_Internal(Entity.EntityID);
            set => SetScale_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Local scale
        /// </summary>
        public Vector3 LocalScale
        {
            get => GetLocalScale_Internal(Entity.EntityID);
            set => SetLocalScale_Internal(Entity.EntityID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetPosition_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetPosition_Internal(uint entityID, Vector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetLocalPosition_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLocalPosition_Internal(uint entityID, Vector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetRotation_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetRotation_Internal(uint entityID, Vector3 rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetLocalRotation_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLocalRotation_Internal(uint entityID, Vector3 rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetScale_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetScale_Internal(uint entityID, Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 GetLocalScale_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLocalScale_Internal(uint entityID, Vector3 scale);
    }
}