using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Methods to work with scenes
    /// </summary>
    public static class SceneManager
    {
        /// <summary>
        /// Creates new empty scene (does not replace current)
        /// </summary>
        /// <param name="name">New scene name</param>
        /// <returns>Created scene</returns>
        /// <remarks>Will replace scene with the same name if it exists</remarks>
        public static Scene CreateNewScene(string name)
        {
            ulong sceneID = CreateNewScene_Internal(name);
            return new Scene(sceneID);
        }
        
        /// <summary>
        /// Delete active scene and replace it with new one
        /// </summary>
        /// <param name="scene">New scene to be active</param>
        public static void SetActiveScene(Scene scene)
        {
            SetActiveScene_Internal(scene?.ID ?? Resource.NULL_RESOURCE_ID);
        }
        
        /// <summary>
        /// Delete active scene and replace it with new one
        /// </summary>
        /// <param name="path">Path to file relative to resources folder</param>
        public static void SetActiveScene(string path)
        {
            SetActiveScene_Internal2(path);
        }
        
        /// <summary>
        /// Gets current active scene
        /// </summary>
        /// <returns>Current active scene</returns>
        public static Scene GetActiveScene()
        {
            ulong sceneID = GetActiveScene_Internal();
            return new Scene(sceneID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong CreateNewScene_Internal(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetActiveScene_Internal(ulong sceneID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetActiveScene_Internal2(string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong GetActiveScene_Internal();
    }
}