using System.Runtime.InteropServices;

namespace Steel
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Mesh
    {
        public Vector3[] Vertices;
        public Color[] Colors;
        public Vector2[] UV;
        public int[] Indices;
    }
}