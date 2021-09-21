namespace Steel
{
    /// <summary>
    /// Type of rigid body, defines how it will be simulated
    /// </summary>
    public enum RigidBodyType
    {
        /// <summary>
        /// No rigid body, it won't be simulated
        /// </summary>
        None,
        
        /// <summary>
        /// Dynamic body is fully simulated, can have velocity and respond to forces
        /// </summary>
        Dynamic,
        
        /// <summary>
        /// Static body does not move under simulation and behaves as if it has infinite mass
        /// </summary>
        Static,
        
        /// <summary>
        /// Kinematic body moves according to its velocity but does not respond to forces
        /// </summary>
        Kinematic,
    }
}