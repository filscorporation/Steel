namespace Iron
{
    /// <summary>
    /// Base class for all resources
    /// </summary>
    public class Resource
    {
        internal const uint NULL_RESOURCE_ID = 0xffffffff;
        
        internal Resource(uint id)
        {
            ID = id;
        }
        
        /// <summary>
        /// Resource unique identificator
        /// </summary>
        public uint ID { get; private set; }
    }
}