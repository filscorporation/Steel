namespace Iron
{
    public abstract class Component
    {
        public ulong ID { get; set; }
        public Entity Entity { get; set; }

        public bool HasComponent<T>() where T : Component, new()
        {
            return Entity.HasComponent<T>();
        }
        
        public T GetComponent<T>() where T : Component, new()
        {
            return Entity.GetComponent<T>();
        }

        public virtual void OnCreate() { }
        public virtual void OnUpdate() { }
        public virtual void OnLateUpdate() { }
        public virtual void OnFixedUpdate() { }
    }
}