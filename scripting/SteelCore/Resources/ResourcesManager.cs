using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Methods to load and access resources (sprites, audio tracks, animations etc.)
    /// </summary>
    public static class ResourcesManager
    {
        /// <summary>
        /// Loads sprite into memory (supported formats: png, aseprite)
        /// </summary>
        /// <param name="path">Path to file relative to resources folder</param>
        /// <returns>Loaded or existing sprite or null if loading failed</returns>
        public static Sprite LoadImage(string path)
        {
            uint spriteID = LoadImage_Internal(path);
            return spriteID == Resource.NULL_RESOURCE_ID ? null : new Sprite(spriteID);
        }

        /// <summary>
        /// Load aseprite file and parse all frames and tags into sprites and animations
        /// </summary>
        /// <param name="path">Path to aseprite file in resources folder</param>
        /// <param name="loopAll">Should all animations be set as loop by default</param>
        /// <returns>Aseprite data or null if loading failed</returns>
        public static AsepriteData LoadAsepriteData(string path, bool loopAll = false)
        {
            uint resourceID = LoadAsepriteData_Internal(path, loopAll);
            return resourceID == Resource.NULL_RESOURCE_ID ? null : new AsepriteData(resourceID);
        }
        
        /// <summary>
        /// Loads audio track in memory (supported formats: wav)
        /// </summary>
        /// <param name="path">Path to file relative to resources folder</param>
        /// <returns>Track or null if loading failed</returns>
        /// <remarks>Loading same track twice will duplicate it in memory</remarks>
        public static AudioTrack LoadAudioTrack(string path)
        {
            uint audioTrackID = LoadAudioTrack_Internal(path);
            return audioTrackID == Resource.NULL_RESOURCE_ID ? null : new AudioTrack(audioTrackID);
        }
        
        /// <summary>
        /// Loads shader in memory (supported language: GLSL)
        /// </summary>
        /// <param name="vsPath">Path to vertex shader file relative to resources folder</param>
        /// <param name="fsPath">Path to fragment shader file relative to resources folder</param>
        /// <returns>Shader or null if loading failed</returns>
        /// <remarks>Loading same shader twice will duplicate it in memory</remarks>
        public static Shader LoadShader(string vsPath, string fsPath)
        {
            uint shaderID = LoadShader_Internal(vsPath, fsPath);
            return shaderID == Resource.NULL_RESOURCE_ID ? null : new Shader(shaderID);
        }
        
        /// <summary>
        /// Creates new material from shader
        /// </summary>
        /// <param name="shader">Shader to initialize material with</param>
        /// <returns>Material or null if creation failed</returns>
        public static Material CreateMaterial(Shader shader)
        {
            uint materialID = CreateMaterial_Internal(shader?.ID ?? Resource.NULL_RESOURCE_ID);
            return materialID == Resource.NULL_RESOURCE_ID ? null : new Material(materialID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint LoadImage_Internal(string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint LoadAsepriteData_Internal(string path, bool loopAll);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint LoadAudioTrack_Internal(string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint LoadShader_Internal(string vsPath, string fsPath);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint CreateMaterial_Internal(uint shaderID);
    }
}