using System;

namespace Steel
{
    /// <summary>
    /// Represents vector in 2D space
    /// </summary>
    public struct Vector2
    {
        /// <summary>
        /// X component
        /// </summary>
        public float X { get; }

        /// <summary>
        /// Y component
        /// </summary>
        public float Y { get; }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }
        
        /// <summary>
        /// Vector with all components set to zero
        /// </summary>
        public static Vector2 Zero => new Vector2(0, 0);
        
        /// <summary>
        /// Vector with all components set to 1
        /// </summary>
        public static Vector2 One => new Vector2(1, 1);

        /// <summary>
        /// Return new vector with X component changed
        /// </summary>
        public Vector2 SetX(float x)
        {
            return new Vector2(x, Y);
        }

        /// <summary>
        /// Return new vector with Y component changed
        /// </summary>
        public Vector2 SetY(float y)
        {
            return new Vector2(X, y);
        }

        /// <summary>
        /// Vectors length (magnitude)
        /// </summary>
        public float Magnitude()
        {
            return Math.Sqrt(X * X + Y * Y);
        }

        /// <summary>
        /// Normalized vector (with magnitude 1)
        /// </summary>
        public Vector2 Normalize()
        {
            float magnitude = Magnitude();
            return Math.Approximately(magnitude, 0.0f) ? Zero : this / magnitude;
        }
        
        public static Vector2 operator +(Vector2 a, Vector2 b) => new Vector2(a.X + b.X, a.Y + b.Y);
        public static Vector2 operator -(Vector2 a, Vector2 b) => new Vector2(a.X - b.X, a.Y - b.Y);
        public static Vector2 operator *(Vector2 a, float l) => new Vector2(a.X * l, a.Y * l);
        public static Vector2 operator /(Vector2 a, float l) => new Vector2(a.X / l, a.Y / l);
        public static Vector2 operator *(float l, Vector2 a) => new Vector2(a.X * l, a.Y * l);
        
        public static bool operator ==(Vector2 a, Vector2 b) => Equals(a, b);
        public static bool operator !=(Vector2 a, Vector2 b) => !Equals(a, b);
        
        public float this[int key]
        {
            get
            {
                switch (key)
                {
                    case 0:
                        return X;
                    case 1:
                        return Y;
                    default:
                        throw new IndexOutOfRangeException();
                }
            }
        }

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
        
        /// <summary>
        /// Returns the angle in radians between the two vectors
        /// </summary>
        public static double Angle(Vector2 vector1, Vector2 vector2)
        {
            float sin = vector1.X * vector2.Y - vector2.X * vector1.Y;  
            float cos = vector1.X * vector2.X + vector1.Y * vector2.Y;
 
            return Math.Atan2(sin, cos);
        }

        /// <summary>
        /// Distance between two vectors
        /// </summary>
        public static float Distance(Vector2 vector1, Vector2 vector2)
        {
            return (vector2 - vector1).Magnitude();
        }
    }
}