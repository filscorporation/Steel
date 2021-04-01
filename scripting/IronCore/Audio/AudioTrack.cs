using System.Runtime.CompilerServices;

namespace Iron
{
    public class AudioTrack
    {
        internal AudioTrack(uint id)
        {
            ID = id;
        }
        
        public uint ID { get; private set; }

        public float Length => GetLength_Internal(ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetLength_Internal(uint audioTrackID);
    }
}