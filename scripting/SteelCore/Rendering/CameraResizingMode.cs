namespace Steel
{
    /// <summary>
    /// Defines how camera will resize when screen or viewport size get changed
    /// </summary>
    public enum CameraResizingMode
    {
        /// <summary>
        /// Camera height will keep the same and width will change
        /// </summary>
        KeepHeight,
        
        /// <summary>
        /// Camera width will keep the same and height will change
        /// </summary>
        KeepWidth,
        
        /// <summary>
        /// Camera size will change to keep viewport proportions
        /// </summary>
        Stretch,
    }
}