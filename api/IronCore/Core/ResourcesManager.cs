using System.Runtime.CompilerServices;

namespace Iron
{
    public static class ResourcesManager
    {
        public static Sprite LoadImage(string path)
        {
            uint spriteID = LoadImage_Internal(path);
            return spriteID == 0 ? null : new Sprite(spriteID);
        }
        
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