using System.Runtime.CompilerServices;

namespace Iron
{
    public class Animator : Component
    {
        public void Play(Animation animation)
        {
            PlayAnimation_Internal(Entity.ID, animation.ID);
        }
        
        public void Play()
        {
            Play_Internal(Entity.ID);
        }
        
        public void Pause()
        {
            Pause_Internal(Entity.ID);
        }
        
        public void Stop()
        {
            Stop_Internal(Entity.ID);
        }
        
        public void Restart()
        {
            Restart_Internal(Entity.ID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void PlayAnimation_Internal(ulong entityID, ulong animationID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Play_Internal(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Pause_Internal(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Stop_Internal(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Restart_Internal(ulong entityID);
    }
}