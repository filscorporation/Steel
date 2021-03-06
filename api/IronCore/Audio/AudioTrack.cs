using System.Runtime.CompilerServices;

namespace Iron
{
    public class AudioTrack
    {
        internal AudioTrack(ulong id)
        {
            ID = id;
        }
        
        public ulong ID { get; private set; }

        public float Length => GetLength_Internal(ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetLength_Internal(ulong audioTrackID);
    }
}