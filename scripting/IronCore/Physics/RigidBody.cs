using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Entity that will be represented in physics world
    /// </summary>
    public class RigidBody : Component
    {
        /// <summary>
        /// Body's mass
        /// </summary>
        public float Mass
        {
            get => GetMass_Internal(Entity.ID);
            set => SetMass_Internal(Entity.ID, value);
        }

        /// <summary>
        /// Type of rigid body to define how it will be simulated. None by default
        /// </summary>
        public RigidBodyType RigidBodyType
        {
            get => GetRigidBodyType_Internal(Entity.ID);
            set => SetRigidBodyType_Internal(Entity.ID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetMass_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetMass_Internal(uint entityID, float mass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern RigidBodyType GetRigidBodyType_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetRigidBodyType_Internal(uint entityID, RigidBodyType type);
    }
}