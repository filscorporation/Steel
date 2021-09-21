namespace Steel
{
    /// <summary>
    /// Information about collision
    /// </summary>
    public struct Collision
    {
        internal Collision(Entity otherEntity)
        {
            OtherEntity = otherEntity;
        }
        
        /// <summary>
        /// Other entity that collided with current
        /// </summary>
        public Entity OtherEntity { get; }

        // TODO: help methods like collision points
    }
}