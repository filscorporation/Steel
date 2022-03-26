using System;

namespace Steel
{
    /// <summary>
    /// Represents vector in 3D space
    /// </summary>
    public struct Vector3
    {
        /// <summary>
        /// X component
        /// </summary>
        public float X { get; }
        
        /// <summary>
        /// Y component
        /// </summary>
        public float Y { get; }
        
        /// <summary>
        /// Z component
        /// </summary>
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
        
        /// <summary>
        /// Vector with all components set to zero
        /// </summary>
        public static Vector3 Zero => new Vector3(0, 0, 0);
        
        /// <summary>
        /// Vector with all components set to 1
        /// </summary>
        public static Vector3 One => new Vector3(1, 1, 1);

        /// <summary>
        /// Return new vector with X component changed
        /// </summary>
        public Vector3 SetX(float x)
        {
            return new Vector3(x, Y, Z);
        }

        /// <summary>
        /// Return new vector with Y component changed
        /// </summary>
        public Vector3 SetY(float y)
        {
            return new Vector3(X, y, Z);
        }

        /// <summary>
        /// Return new vector with Z component changed
        /// </summary>
        public Vector3 SetZ(float z)
        {
            return new Vector3(X, Y, z);
        }

        /// <summary>
        /// Vectors length (magnitude)
        /// </summary>
        public float Magnitude()
        {
            return Math.Sqrt(X * X + Y * Y + Z * Z);
        }

        /// <summary>
        /// Normalized vector (with magnitude 1)
        /// </summary>
        public Vector3 Normalize()
        {
            float magnitude = Magnitude();
            return Math.Approximately(magnitude, 0.0f) ? Zero : this / magnitude;
        }
        
        public static Vector3 operator +(Vector3 a, Vector3 b) => new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        public static Vector3 operator -(Vector3 a, Vector3 b) => new Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        public static Vector3 operator *(Vector3 a, float l) => new Vector3(a.X * l, a.Y * l, a.Z * l);
        public static Vector3 operator /(Vector3 a, float l) => new Vector3(a.X / l, a.Y / l, a.Z / l);
        public static Vector3 operator *(float l, Vector3 a) => new Vector3(a.X * l, a.Y * l, a.Z * l);
        
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
                    case 2:
                        return Z;
                    default:
                        throw new IndexOutOfRangeException();
                }
            }
        }
        
        public static bool operator ==(Vector3 a, Vector3 b) => Equals(a, b);
        public static bool operator !=(Vector3 a, Vector3 b) => !Equals(a, b);

        public override bool Equals(object obj)
        {
            if (obj is Vector3 vec)
                return Math.Approximately(X, vec.X) && Math.Approximately(Y, vec.Y) && Math.Approximately(Z, vec.Z);
            
            return base.Equals(obj);
        }
        
        public static implicit operator Vector3(Vector2 vector2)
        {
            return new Vector3(vector2.X, vector2.Y);
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

        /// <summary>
        /// Angle in radians between the two vectors
        /// </summary>
        public static float Angle(Vector3 vector1, Vector3 vector2)
        {
            return Math.Acos(Dot(vector1, vector2) / (vector1.Magnitude() * vector2.Magnitude()));
        }

        /// <summary>
        /// Cross product of two vectors
        /// </summary>
        public static Vector3 Cross(Vector3 vector1, Vector3 vector2)
        {
            return new Vector3(
                vector1.Y * vector2.Z - vector2.Y * vector1.Z,
                vector2.X * vector1.Z - vector1.X * vector2.Z,
                vector1.X * vector2.Y - vector2.X * vector1.Y
            );
        }

        /// <summary>
        /// Distance between two vectors
        /// </summary>
        public static float Distance(Vector3 vector1, Vector3 vector2)
        {
            return (vector2 - vector1).Magnitude();
        }

        /// <summary>
        /// Dot product of two vectors
        /// </summary>
        public static float Dot(Vector3 vector1, Vector3 vector2)
        {
            return vector1.X * vector2.Y + vector1.Y * vector2.Y + vector1.Z * vector2.Z;
        }
    }
}