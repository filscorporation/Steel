namespace Steel
{
    /// <summary>
    /// Operation on stencil buffer values
    /// </summary>
    public enum StencilOperation
    {
        Keep,
        Zero,
        Replace,
        Increment,
        IncrementWrap,
        Decrement,
        DecrementWrap,
        Invert,
    }
}