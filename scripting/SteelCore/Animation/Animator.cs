using System.Runtime.CompilerServices;

namespace Steel
{
    /// <summary>
    /// Components used to play <see cref="Animation"/>s
    /// </summary>
    public class Animator : Component
    {
        /// <summary>
        /// Adds new animation if needed and plays it
        /// </summary>
        /// <param name="animation">Animation to play</param>
        public void Play(Animation animation)
        {
            PlayAnimation_Internal(Entity.EntityID, animation.ID);
        }
        
        /// <summary>
        /// Play existing in animator animation by name
        /// </summary>
        /// <param name="animationName">Animation to play name</param>
        public void Play(string animationName)
        {
            PlayAnimation_Internal2(Entity.EntityID, animationName);
        }
        
        /// <summary>
        /// Play or continue current animation
        /// </summary>
        public void Play()
        {
            Play_Internal(Entity.EntityID);
        }
        
        /// <summary>
        /// Pause animation
        /// </summary>
        public void Pause()
        {
            Pause_Internal(Entity.EntityID);
        }
        
        /// <summary>
        /// Stop playing animation
        /// </summary>
        public void Stop()
        {
            Stop_Internal(Entity.EntityID);
        }
        
        /// <summary>
        /// Play current animation from the start
        /// </summary>
        public void Restart()
        {
            Restart_Internal(Entity.EntityID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void PlayAnimation_Internal(uint entityID, ulong animationID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void PlayAnimation_Internal2(uint entityID, string animationName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Play_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Pause_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Stop_Internal(uint entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Restart_Internal(uint entityID);
    }
}