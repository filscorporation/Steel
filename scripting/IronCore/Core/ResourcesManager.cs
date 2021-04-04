using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Methods to load and access resources (sprites, audio tracks, animations etc.)
    /// </summary>
    public static class ResourcesManager
    {
        /// <summary>
        /// Loads sprite into memory (supported formats: png)
        /// </summary>
        /// <param name="path">Path to file relative to resources folder</param>
        /// <returns>Loaded sprite or null if loading failed</returns>
        /// <remarks>Loading same sprite twice will duplicate it in memory</remarks>
        public static Sprite LoadImage(string path)
        {
            uint spriteID = LoadImage_Internal(path);
            return spriteID == 0 ? null : new Sprite(spriteID);
        }
        
        /// <summary>
        /// Loads audio track in memory (supported formats: wav)
        /// </summary>
        /// <param name="path">Path to file relative to resources folder</param>
        /// <returns></returns>
        /// <remarks>Loading same track twice will duplicate it in memory</remarks>
        public static AudioTrack LoadAudioTrack(string path)
        {
            uint audioTrackID = LoadAudioTrack_Internal(path);
            return audioTrackID == 0 ? null : new AudioTrack(audioTrackID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint LoadImage_Internal(string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint LoadAudioTrack_Internal(string path);
    }
}