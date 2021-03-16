namespace Iron
{
    public abstract class ScriptComponent : Component
    {
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