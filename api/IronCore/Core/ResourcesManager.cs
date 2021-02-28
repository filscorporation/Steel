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

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern ulong LoadImage_Internal(string path);
    }
}