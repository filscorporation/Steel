namespace Iron
{
    public abstract class UIComponent : Component
    {
        public RectTransformation RectTransform => GetComponent<RectTransformation>();
    }
}