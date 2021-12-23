using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Entity that will be represented in physics world
    /// </summary>
    public class RigidBody : Component
    {
        /// <summary>
        /// Type of rigid body to define how it will be simulated. None by default
        /// </summary>
        public RigidBodyType RigidBodyType
        {
            get => GetRigidBodyType_Internal(Entity.EntityID);
            set => SetRigidBodyType_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Body's mass
        /// </summary>
        public float Mass
        {
            get => GetMass_Internal(Entity.EntityID);
            set => SetMass_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Linear velocity
        /// </summary>
        public Vector2 Velocity
        {
            get
            {
                GetVelocity_Internal(Entity.EntityID, out Vector2 velocity);
                return velocity;
            }
            set => SetVelocity_Internal(Entity.EntityID, ref value);
        }
        
        /// <summary>
        /// Angular velocity
        /// </summary>
        public float AngularVelocity
        {
            get => GetAngularVelocity_Internal(Entity.EntityID);
            set => SetAngularVelocity_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Scale of worlds gravity vector applied to this body
        /// </summary>
        public float GravityScale
        {
            get => GetGravityScale_Internal(Entity.EntityID);
            set => SetGravityScale_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Body's colliders friction (usually between 0 and 1)
        /// </summary>
        public float Friction
        {
            get => GetFriction_Internal(Entity.EntityID);
            set => SetFriction_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// How bouncy is body (0 is not and 1 means it keeps all the energy)
        /// </summary>
        public float Restitution
        {
            get => GetRestitution_Internal(Entity.EntityID);
            set => SetRestitution_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Linear damping is used to reduce the world velocity of bodies
        /// </summary>
        public float LinearDamping
        {
            get => GetLinearDamping_Internal(Entity.EntityID);
            set => SetLinearDamping_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Angular damping is used to reduce the torque of bodies
        /// </summary>
        public float AngularDamping
        {
            get => GetAngularDamping_Internal(Entity.EntityID);
            set => SetAngularDamping_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Fix rotation of the body
        /// </summary>
        public bool FixedRotation
        {
            get => GetFixedRotation_Internal(Entity.EntityID);
            set => SetFixedRotation_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Using CCD will help fast moving objects to collide properly and not tunnel though other bodies
        /// </summary>
        public bool UseContinuousCollisionDetection
        {
            get => GetUseContinuousCollisionDetection_Internal(Entity.EntityID);
            set => SetUseContinuousCollisionDetection_Internal(Entity.EntityID, value);
        }
        
        /// <summary>
        /// Apply a force to the center of mass
        /// </summary>
        /// <param name="force">Force vector</param>
        public void ApplyForce(Vector2 force) => ApplyForce_Internal(Entity.EntityID, ref force);
        
        /// <summary>
        /// Apply a force at a world point
        /// </summary>
        /// <param name="force">Force vector</param>
        /// <param name="point">Point where force is applied</param>
        public void ApplyForce(Vector2 force, Vector2 point) => ApplyForce_Internal2(Entity.EntityID, ref force, ref point);
        
        /// <summary>
        /// Apply a torque
        /// </summary>
        /// <param name="torque">Torque value (positive is clockwise)</param>
        public void ApplyTorque(float torque) => ApplyTorque_Internal(Entity.EntityID, torque);
        
        /// <summary>
        /// Apply an impulse to the center of mass
        /// </summary>
        /// <param name="impulse">Impulse vector</param>
        public void ApplyLinearImpulse(Vector2 impulse) => ApplyLinearImpulse_Internal(Entity.EntityID, ref impulse);
        
        /// <summary>
        /// Apply an impulse at a point
        /// </summary>
        /// <param name="impulse">Impulse vector</param>
        /// <param name="point">Point where impulse is applied</param>
        public void ApplyLinearImpulse(Vector2 impulse, Vector2 point) => ApplyLinearImpulse_Internal2(Entity.EntityID, ref impulse, ref point);
        
        /// <summary>
        /// Apply an angular impulse
        /// </summary>
        /// <param name="torque">Impulse value (positive is clockwise)</param>
        public void ApplyAngularImpulse(float torque) => ApplyAngularImpulse_Internal(Entity.EntityID, torque);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern RigidBodyType GetRigidBodyType_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetRigidBodyType_Internal(uint entityID, RigidBodyType type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetMass_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetMass_Internal(uint entityID, float mass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetVelocity_Internal(uint entityID, out Vector2 velocity);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetVelocity_Internal(uint entityID, ref Vector2 velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetAngularVelocity_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAngularVelocity_Internal(uint entityID, float velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetGravityScale_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetGravityScale_Internal(uint entityID, float gravityScale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetFriction_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetFriction_Internal(uint entityID, float friction);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetRestitution_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetRestitution_Internal(uint entityID, float restitution);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetLinearDamping_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLinearDamping_Internal(uint entityID, float damping);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetAngularDamping_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetAngularDamping_Internal(uint entityID, float damping);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetFixedRotation_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetFixedRotation_Internal(uint entityID, bool isFixed);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetUseContinuousCollisionDetection_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetUseContinuousCollisionDetection_Internal(uint entityID, bool useCCD);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void ApplyForce_Internal(uint entityID, ref Vector2 force);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void ApplyForce_Internal2(uint entityID, ref Vector2 force, ref Vector2 point);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void ApplyTorque_Internal(uint entityID, float torque);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void ApplyLinearImpulse_Internal(uint entityID, ref Vector2 impulse);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void ApplyLinearImpulse_Internal2(uint entityID, ref Vector2 impulse, ref Vector2 point);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void ApplyAngularImpulse_Internal(uint entityID, float impulse);
    }
}