using System;

namespace Steel
{
    /// <summary>
    /// Mark script field as serializable by engine
    /// </summary>
    [AttributeUsage(AttributeTargets.Field)]
    public class SerializeFieldAttribute : Attribute
    {
        
    }
}