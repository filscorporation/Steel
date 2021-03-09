using System.Runtime.CompilerServices;

namespace Iron
{
    public class AudioSource : Component
    {
        public float Volume
        {
            get => GetVolume_Internal(Entity.ID);
            set => SetVolume_Internal(Entity.ID, value);
        }

        public bool Loop
        {
            get => GetLoop_Internal(Entity.ID);
            set => SetLoop_Internal(Entity.ID, value);
        }

        public void Play(AudioTrack audioTrack) => Play_Internal(Entity.ID, audioTrack?.ID ?? 0);

        public void Stop() => Stop_Internal(Entity.ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetVolume_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float SetVolume_Internal(uint entityID, float volume);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetLoop_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLoop_Internal(uint entityID, bool loop);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Play_Internal(uint entityID, uint audioTrackID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Stop_Internal(uint entityID);
    }
}