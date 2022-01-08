using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Loaded audio track
    /// </summary>
    public class AudioTrack : Resource
    {
        internal AudioTrack(ulong id) : base(id) { }

        /// <summary>
        /// Track length in seconds
        /// </summary>
        public float Length => GetLength_Internal(ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetLength_Internal(ulong audioTrackID);
    }
}