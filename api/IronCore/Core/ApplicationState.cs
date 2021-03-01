namespace Iron
{
    public enum ApplicationState
    {
        Initializing,
        EntryPoint,
        PollEvents,
        OnUpdate,
        OnPhysicsUpdate,
        OnLateUpdate,
        OnRender,
        CleaningDestroyedEntities,
    }
}