namespace Iron
{
    public struct Collision
    {
        internal Collision(Entity otherEntity)
        {
            OtherEntity = otherEntity;
        }
        
        public Entity OtherEntity { get; }

        // TODO: help methods like collision points
    }
}