using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Component used to play <see cref="AudioTrack"/>s in the scene
    /// </summary>
    public class AudioSource : Component
    {
        /// <summary>
        /// Audio source volume
        /// </summary>
        public float Volume
        {
            get => GetVolume_Internal(Entity.EntityID);
            set => SetVolume_Internal(Entity.EntityID, value);
        }

        /// <summary>
        /// Should it loop audio track it is playing
        /// </summary>
        public bool Loop
        {
            get => GetLoop_Internal(Entity.EntityID);
            set => SetLoop_Internal(Entity.EntityID, value);
        }

        /// <summary>
        /// Play audio track
        /// </summary>
        /// <param name="audioTrack">Track to play</param>
        public void Play(AudioTrack audioTrack) => Play_Internal(Entity.EntityID, audioTrack?.ID ?? Resource.NULL_RESOURCE_ID);

        /// <summary>
        /// Stops playing audio track
        /// </summary>
        public void Stop() => Stop_Internal(Entity.EntityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetVolume_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float SetVolume_Internal(uint entityID, float volume);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetLoop_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLoop_Internal(uint entityID, bool loop);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Play_Internal(uint entityID, ulong audioTrackID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Stop_Internal(uint entityID);
    }
}