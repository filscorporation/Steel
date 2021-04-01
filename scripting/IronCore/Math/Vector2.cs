namespace Iron
{
    public struct Vector2
    {
        public float X { get; }

        public float Y { get; }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }
        
        public static Vector2 Zero => new Vector2(0, 0);
        public static Vector2 One => new Vector2(1, 1);

        public Vector2 SetX(float x)
        {
            return new Vector2(x, Y);
        }

        public Vector2 SetY(float y)
        {
            return new Vector2(X, y);
        }
        
        public static Vector2 operator +(Vector2 a, Vector2 b) => new Vector2(a.X + b.X, a.Y + b.Y);
        public static Vector2 operator -(Vector2 a, Vector2 b) => new Vector2(a.X - b.X, a.Y - b.Y);
        public static Vector2 operator *(Vector2 a, float l) => new Vector2(a.X * l, a.Y * l);
        public static Vector2 operator /(Vector2 a, float l) => new Vector2(a.X / l, a.Y / l);
        public static Vector2 operator *(float l, Vector2 a) => new Vector2(a.X * l, a.Y * l);
        
        public static bool operator ==(Vector2 a, Vector2 b) => Equals(a, b);
        public static bool operator !=(Vector2 a, Vector2 b) => !Equals(a, b);

        public override bool Equals(object obj)
        {
            if (obj is Vector2 vec)
                return Math.Approximately(X, vec.X) && Math.Approximately(Y, vec.Y);
            
            return base.Equals(obj);
        }
        
        public static implicit operator Vector2(Vector3 vector3)
        {
            return new Vector2(vector3.X, vector3.Y);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                int result = 0;
                result = (result * 397) ^ X.GetHashCode();
                result = (result * 397) ^ Y.GetHashCode();
                
                return result;
            }
        }

        public override string ToString()
        {
            return $"({X}; {Y})";
        }
    }
}