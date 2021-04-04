namespace Iron
{
    /// <summary>
    /// Base class for all UI components
    /// </summary>
    public abstract class UIComponent : Component
    {
        /// <summary>
        /// UI elements rect transformation
        /// </summary>
        public RectTransformation RectTransform => GetComponent<RectTransformation>();
    }
}