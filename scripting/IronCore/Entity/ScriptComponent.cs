namespace Iron
{
    /// <summary>
    /// Basic class from which all custom script components should inherit. Script component should be created by
    /// attaching to entity to be updated and receive events
    /// </summary>
    public abstract class ScriptComponent : Component
    {
        /// <summary>
        /// Called after component was attached to entity
        /// </summary>
        public virtual void OnCreate() { }
        
        /// <summary>
        /// Called before component is removed from entity
        /// </summary>
        public virtual void OnDestroy() { }
        
        /// <summary>
        /// Called once per frame
        /// </summary>
        public virtual void OnUpdate() { }
        
        /// <summary>
        /// Called after all updates
        /// </summary>
        public virtual void OnLateUpdate() { }
        
        /// <summary>
        /// Called in physics update with fixed time step
        /// </summary>
        public virtual void OnFixedUpdate() { }
        
        /// <summary>
        /// Called after component became active in hierarchy
        /// </summary>
        public virtual void OnEnabled() { }
        
        /// <summary>
        /// Called after component became inactive in hierarchy
        /// </summary>
        public virtual void OnDisabled() { }
        
        /// <summary>
        /// Called if <see cref="RigidBody"/> attached to this entity entered collision with another body
        /// </summary>
        /// <param name="collision">Information about collision</param>
        public virtual void OnCollisionEnter(Collision collision) { }
        
        /// <summary>
        /// Called every frame <see cref="RigidBody"/> attached to this entity stays in collision with another body
        /// </summary>
        /// <param name="collision">Information about collision</param>
        public virtual void OnCollisionStay(Collision collision) { }
        
        /// <summary>
        /// Called after <see cref="RigidBody"/> attached to this entity exited collision with another body
        /// </summary>
        /// <param name="collision">Information about collision</param>
        public virtual void OnCollisionExit(Collision collision) { }
        
        /// <summary>
        /// Called first frame when mouse entered <see cref="Collider"/> attached to this entity
        /// </summary>
        public virtual void OnMouseOver() { }
        
        /// <summary>
        /// Called every frame while mouse stays in <see cref="Collider"/> attached to this entity
        /// </summary>
        public virtual void OnMouseEnter() { }
        
        /// <summary>
        /// Called first frame when mouse exited <see cref="Collider"/> attached to this entity
        /// </summary>
        public virtual void OnMouseExit() { }
        
        /// <summary>
        /// Called every frame while mouse stays pressed while inside <see cref="Collider"/> attached to this entity
        /// </summary>
        public virtual void OnMousePressed() { }
        
        /// <summary>
        /// Called when mouse just got pressed while inside <see cref="Collider"/> attached to this entity
        /// </summary>
        public virtual void OnMouseJustPressed() { }
        
        /// <summary>
        /// Called when mouse just got released while inside <see cref="Collider"/> attached to this entity
        /// </summary>
        public virtual void OnMouseJustReleased() { }
        
        /// <summary>
        /// Called first frame when mouse entered UI event handler attached to this entity
        /// </summary>
        public virtual void OnMouseOverUI() { }
        
        /// <summary>
        /// Called every frame while mouse stays in UI event handler attached to this entity
        /// </summary>
        public virtual void OnMouseEnterUI() { }
        
        /// <summary>
        /// Called first frame when mouse exited UI event handler attached to this entity
        /// </summary>
        public virtual void OnMouseExitUI() { }
        
        /// <summary>
        /// Called every frame while mouse stays pressed while inside UI event handler attached to this entity
        /// </summary>
        public virtual void OnMousePressedUI() { }
        
        /// <summary>
        /// Called when mouse just got pressed while inside UI event handler attached to this entity
        /// </summary>
        public virtual void OnMouseJustPressedUI() { }
        
        /// <summary>
        /// Called when mouse just got released while inside UI event handler attached to this entity
        /// </summary>
        public virtual void OnMouseJustReleasedUI() { }
    }
}