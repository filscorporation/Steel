using System.Runtime.CompilerServices;

namespace Iron
{
    public static class ResourcesManager
    {
        public static Sprite LoadImage(string path)
        {
            ulong spriteID = LoadImage_Internal(path);
            return spriteID == 0 ? null : new Sprite(spriteID);
        }
        
        public static AudioTrack LoadAudioTrack(string path)
        {
            ulong audioTrackID = LoadAudioTrack_Internal(path);
            return audioTrackID == 0 ? null : new AudioTrack(audioTrackID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong LoadImage_Internal(string path);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong LoadAudioTrack_Internal(string path);
    }
}