using System.Runtime.CompilerServices;

namespace Iron
{
    /// <summary>
    /// Component that receives input from <see cref="AudioSource"/>s in the scene
    /// </summary>
    /// <remarks>There should be only one in the scene, by default it is initialized attached to main <see cref="Camera"/></remarks>
    public class AudioListener : Component
    {
        /// <summary>
        /// Listener global volume
        /// </summary>
        public float Volume
        {
            get => GetVolume_Internal(Entity.ID);
            set => SetVolume_Internal(Entity.ID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float GetVolume_Internal(uint entityID);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float SetVolume_Internal(uint entityID, float volume);
    }
}