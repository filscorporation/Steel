using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Methods to get resources (sprites, audio tracks, animations etc.)
    /// </summary>
    public static class ResourcesManager
    {
        /// <summary>
        /// Get sprite from loaded resources (supported formats: png, aseprite)
        /// </summary>
        /// <param name="path">Path to file relative to resources folder</param>
        /// <returns>Resource or null if not found</returns>
        public static Sprite GetImage(string path)
        {
            ulong spriteID = GetImage_Internal(path);
            return spriteID == Resource.NULL_RESOURCE_ID ? null : new Sprite(spriteID);
        }

        /// <summary>
        /// Get aseprite data (with all frames and tags parsed into sprites and animations)
        /// </summary>
        /// <param name="path">Path to aseprite file relative to resources folder</param>
        /// <param name="loopAll">Should all animations be set as loop by default</param>
        /// <returns>Resource or null if not found</returns>
        public static AsepriteData GetAsepriteData(string path, bool loopAll = false)
        {
            ulong resourceID = GetAsepriteData_Internal(path, loopAll);
            return resourceID == Resource.NULL_RESOURCE_ID ? null : new AsepriteData(resourceID);
        }
        
        /// <summary>
        /// Get audio track from loaded resources (supported formats: wav)
        /// </summary>
        /// <param name="path">Path to file relative to resources folder</param>
        /// <returns>Resource or null if not found</returns>
        public static AudioTrack GetAudioTrack(string path)
        {
            ulong audioTrackID = GetAudioTrack_Internal(path);
            return audioTrackID == Resource.NULL_RESOURCE_ID ? null : new AudioTrack(audioTrackID);
        }
        
        /// <summary>
        /// Get shader from loaded resources (supported language: GLSL)
        /// </summary>
        /// <param name="vsPath">Path to vertex shader file relative to resources folder</param>
        /// <param name="fsPath">Path to fragment shader file relative to resources folder</param>
        /// <returns>Resource or null if not found</returns>
        public static Shader GetShader(string vsPath, string fsPath)
        {
            ulong shaderID = GetShader_Internal(vsPath, fsPath);
            return shaderID == Resource.NULL_RESOURCE_ID ? null : new Shader(shaderID);
        }
        
        /// <summary>
        /// Create new material from shader
        /// </summary>
        /// <param name="shader">Shader to initialize material with</param>
        /// <returns>Material or null if creation failed</returns>
        public static Material CreateMaterial(Shader shader)
        {
            ulong materialID = CreateMaterial_Internal(shader?.ID ?? Resource.NULL_RESOURCE_ID);
            return materialID == Resource.NULL_RESOURCE_ID ? null : new Material(materialID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong GetImage_Internal(string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong GetAsepriteData_Internal(string path, bool loopAll);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong GetAudioTrack_Internal(string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong GetShader_Internal(string vsPath, string fsPath);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong CreateMaterial_Internal(ulong shaderID);
    }
}