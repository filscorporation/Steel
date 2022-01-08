namespace Steel
{
    /// <summary>
    /// Base class for all resources
    /// </summary>
    public class Resource
    {
        internal const ulong NULL_RESOURCE_ID = 0xffffffffffffffff;
        
        internal Resource(ulong id)
        {
            ID = id;
        }
        
        /// <summary>
        /// Resource unique identificator
        /// </summary>
        public ulong ID { get; private set; }
    }
}