namespace Iron.Math
{
    public struct Vector3
    {
        public float X { get; }
        public float Y { get; }
        public float Z { get; }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector3(float x, float y)
        {
            X = x;
            Y = y;
            Z = 0;
        }
        
        public static Vector3 Zero => new Vector3(0, 0, 0);
        public static Vector3 One => new Vector3(1, 1, 1);

        public Vector3 SetX(float x)
        {
            return new Vector3(x, Y, Z);
        }

        public Vector3 SetY(float y)
        {
            return new Vector3(X, y, Z);
        }

        public Vector3 SetZ(float z)
        {
            return new Vector3(X, Y, z);
        }
        
        public static Vector3 operator +(Vector3 a, Vector3 b) => new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        public static Vector3 operator -(Vector3 a, Vector3 b) => new Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        public static Vector3 operator *(Vector3 a, float l) => new Vector3(a.X * l, a.Y * l, a.Z * l);
        public static Vector3 operator /(Vector3 a, float l) => new Vector3(a.X / l, a.Y / l, a.Z / l);
        public static Vector3 operator *(float l, Vector3 a) => new Vector3(a.X * l, a.Y * l, a.Z * l);
        public static Vector3 operator /(float l, Vector3 a) => new Vector3(a.X / l, a.Y / l, a.Z / l);
        
        public static bool operator ==(Vector3 a, Vector3 b) => Equals(a, b);
        public static bool operator !=(Vector3 a, Vector3 b) => !Equals(a, b);

        public override bool Equals(object obj)
        {
            if (obj is Vector3 vec)
                return Math.Approximately(X, vec.X) && Math.Approximately(Y, vec.Y) && Math.Approximately(Z, vec.Z);
            
            return base.Equals(obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                int result = 0;
                result = (result * 397) ^ X.GetHashCode();
                result = (result * 397) ^ Y.GetHashCode();
                result = (result * 397) ^ Z.GetHashCode();
                
                return result;
            }
        }

        public override string ToString()
        {
            return $"({X}; {Y}; {Z})";
        }
    }
}