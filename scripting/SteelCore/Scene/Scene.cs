using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Collection of currently updated and rendered entities and their context
    /// </summary>
    public class Scene : Resource
    {
        internal Scene(ulong id) : base(id) { }

        /// <summary>
        /// Texture ID in GPU
        /// </summary>
        public string Name => GetName_Internal(ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetName_Internal(ulong sceneID);
    }
}