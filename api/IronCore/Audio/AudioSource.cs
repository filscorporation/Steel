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
            get => GetLoop_Internal(ID);
            set => SetLoop_Internal(ID, value);
        }

        public void Play(AudioTrack audioTrack) => Play_Internal(Entity.ID, audioTrack?.ID ?? 0);

        public void Stop() => Stop_Internal(Entity.ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetVolume_Internal(ulong entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float SetVolume_Internal(ulong entityID, float volume);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetLoop_Internal(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetLoop_Internal(ulong entityID, bool loop);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Play_Internal(ulong entityID, ulong audioTrackID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float Stop_Internal(ulong entityID);
    }
}