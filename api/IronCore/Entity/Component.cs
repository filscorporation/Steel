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

        public virtual void OnCreate() { }
        public virtual void OnUpdate() { }
        public virtual void OnLateUpdate() { }
        public virtual void OnFixedUpdate() { }
        public virtual void OnCollisionEnter(Collision collision) { }
        public virtual void OnCollisionStay(Collision collision) { }
        public virtual void OnCollisionExit(Collision collision) { }
        public virtual void OnMouseOver() { }
        public virtual void OnMouseEnter() { }
        public virtual void OnMouseExit() { }
        public virtual void OnMousePressed() { }
        public virtual void OnMouseJustPressed() { }
        public virtual void OnMouseJustReleased() { }
    }
}