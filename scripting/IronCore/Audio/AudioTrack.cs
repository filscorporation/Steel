using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Loaded audio track
    /// </summary>
    public class AudioTrack
    {
        internal AudioTrack(uint id)
        {
            ID = id;
        }
        
        /// <summary>
        /// Resource unique identificator
        /// </summary>
        public uint ID { get; private set; }

        /// <summary>
        /// Track length in seconds
        /// </summary>
        public float Length => GetLength_Internal(ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetLength_Internal(uint audioTrackID);
    }
}