using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Methods to load and access resources (sprites, audio tracks, animations etc.)
    /// </summary>
    public static class ResourcesManager
    {
        /// <summary>
        /// Loads sprite into memory or returns if already loaded (supported formats: png, aseprite)
        /// </summary>
        /// <param name="path">Path to file relative to resources folder</param>
        /// <returns>Loaded or null if loading failed</returns>
        public static Sprite LoadImage(string path)
        {
            ulong spriteID = LoadImage_Internal(path);
            return spriteID == Resource.NULL_RESOURCE_ID ? null : new Sprite(spriteID);
        }

        /// <summary>
        /// Load aseprite file and parse all frames and tags into sprites and animations or returns if already loaded
        /// </summary>
        /// <param name="path">Path to aseprite file in resources folder</param>
        /// <param name="loopAll">Should all animations be set as loop by default</param>
        /// <returns>Aseprite data or null if loading failed</returns>
        public static AsepriteData LoadAsepriteData(string path, bool loopAll = false)
        {
            ulong resourceID = LoadAsepriteData_Internal(path, loopAll);
            return resourceID == Resource.NULL_RESOURCE_ID ? null : new AsepriteData(resourceID);
        }
        
        /// <summary>
        /// Loads audio track in memory or returns if already loaded (supported formats: wav)
        /// </summary>
        /// <param name="path">Path to file relative to resources folder</param>
        /// <returns>Track or null if loading failed</returns>
        public static AudioTrack LoadAudioTrack(string path)
        {
            ulong audioTrackID = LoadAudioTrack_Internal(path);
            return audioTrackID == Resource.NULL_RESOURCE_ID ? null : new AudioTrack(audioTrackID);
        }
        
        /// <summary>
        /// Loads shader in memory or returns if already loaded (supported language: GLSL)
        /// </summary>
        /// <param name="vsPath">Path to vertex shader file relative to resources folder</param>
        /// <param name="fsPath">Path to fragment shader file relative to resources folder</param>
        /// <returns>Shader or null if loading failed</returns>
        public static Shader LoadShader(string vsPath, string fsPath)
        {
            ulong shaderID = LoadShader_Internal(vsPath, fsPath);
            return shaderID == Resource.NULL_RESOURCE_ID ? null : new Shader(shaderID);
        }
        
        /// <summary>
        /// Creates new material from shader
        /// </summary>
        /// <param name="shader">Shader to initialize material with</param>
        /// <returns>Material or null if creation failed</returns>
        public static Material CreateMaterial(Shader shader)
        {
            ulong materialID = CreateMaterial_Internal(shader?.ID ?? Resource.NULL_RESOURCE_ID);
            return materialID == Resource.NULL_RESOURCE_ID ? null : new Material(materialID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong LoadImage_Internal(string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong LoadAsepriteData_Internal(string path, bool loopAll);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong LoadAudioTrack_Internal(string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong LoadShader_Internal(string vsPath, string fsPath);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong CreateMaterial_Internal(ulong shaderID);
    }
}