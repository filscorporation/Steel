namespace Iron
{
    /// <summary>
    /// Application execution state
    /// </summary>
    public enum ApplicationState
    {
        /// <summary>
        /// Initialization of engine and all systems
        /// </summary>
        Initializing,
        
        /// <summary>
        /// Scripting API entry point
        /// </summary>
        EntryPoint,
        
        /// <summary>
        /// Polling input events
        /// </summary>
        PollEvents,
        
        /// <summary>
        /// Main systems and scripts update
        /// </summary>
        OnUpdate,
        
        /// <summary>
        /// Physics update in fixed time step
        /// </summary>
        OnPhysicsUpdate,
        
        /// <summary>
        /// Scripts late update after regular update
        /// </summary>
        OnLateUpdate,
        
        /// <summary>
        /// Rendering scene
        /// </summary>
        OnRender,
        
        /// <summary>
        /// Clearing all entities destroyed in this frame
        /// </summary>
        CleaningDestroyedEntities,
    }
}