namespace Steel
{
    /// <summary>
    /// Function used in stencil/depth test to compare with reference value
    /// </summary>
    public enum ComparisonFunction
    {
        Never,
        Less,
        LessEqual,
        Greater,
        GreaterEqual,
        Equal,
        NotEqual,
        Always,
    }
}