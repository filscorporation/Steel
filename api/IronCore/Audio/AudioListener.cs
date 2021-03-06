using System.Runtime.CompilerServices;

namespace Iron
{
    public class AudioListener : Component
    {
        public float Volume
        {
            get => GetVolume_Internal(Entity.ID);
            set => SetVolume_Internal(Entity.ID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetVolume_Internal(ulong entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float SetVolume_Internal(ulong entityID, float volume);
    }
}