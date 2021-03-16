namespace Iron
{
    public abstract class Component
    {
        internal Component() { }
        
        public Entity Entity { get; set; }
        
        public Transformation Transformation => GetComponent<Transformation>();

        public bool HasComponent<T>() where T : Component, new()
        {
            return Entity.HasComponent<T>();
        }
        
        public T GetComponent<T>() where T : Component, new()
        {
            return Entity.GetComponent<T>();
        }
    }
}