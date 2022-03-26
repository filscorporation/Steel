namespace Steel
{
    /// <summary>
    /// Mathematics functions with float numbers
    /// </summary>
    public static class Math
    {
        public static float Eps = 0.000001f;
        
        public static float Pi = 3.1415926536f;
        
        /// <summary>
        /// Degrees to radians multiplicator
        /// </summary>
        public static float DegToRad = Pi / 180f;
        
        
        /// <summary>
        /// Radians to degrees multiplicator
        /// </summary>
        public static float RadToDeg = 180f / Pi;

        /// <summary>
        /// Absolute value
        /// </summary>
        public static float Abs(float a)
        {
            return a < 0 ? -a : a;
        }

        /// <summary>
        /// Arc-cosine of angle in radians
        /// </summary>
        public static float Acos(float cos)
        {
            return (float)System.Math.Acos(cos);
        }

        /// <summary>
        /// Arc-sine of angle in radians
        /// </summary>
        public static float Asin(float sin)
        {
            return (float)System.Math.Asin(sin);
        }

        /// <summary>
        /// Arc-tangent of angle in radians
        /// </summary>
        public static float Atan(float tan)
        {
            return (float)System.Math.Atan(tan);
        }

        /// <summary>
        /// Arc-cosine of angle a / b in radians
        /// </summary>
        public static float Atan2(float a, float b)
        {
            return (float)System.Math.Atan2(a, b);
        }
        
        /// <summary>
        /// Compares to float numbers
        /// </summary>
        public static bool Approximately(float a, float b)
        {
            return Abs(a - b) < Eps;
        }

        /// <summary>
        /// Returns the smallest integral value greater than or equal to the specified number
        /// </summary>
        public static float Ceiling(float a)
        {
            return (float)System.Math.Ceiling(a);
        }

        /// <summary>
        /// Clamps value between min and max
        /// </summary>
        public static float Clamp(float a, float min, float max)
        {
            if (max <= min) return min;
            return a <= min ? min : a >= max ? max : a;
        }

        /// <summary>
        /// Clamps value between min and max
        /// </summary>
        public static int Clamp(int a, int min, int max)
        {
            if (max <= min) return min;
            return a <= min ? min : a >= max ? max : a;
        }

        /// <summary>
        /// Clamps value between 0 and 1
        /// </summary>
        public static float Clamp01(float a)
        {
            return a <= 0 ? 0 : a >= 1 ? 1 : a;
        }

        /// <summary>
        /// Cosine of angle in radians
        /// </summary>
        public static float Cos(float angle)
        {
            return (float)System.Math.Cos(angle);
        }

        /// <summary>
        /// Distance in degrees between to angles (in -180 to 180 range)
        /// </summary>
        public static float DeltaAngle(float from, float to)
        {
            float delta = from - to;
            while (delta < -180) delta += 360;
            while (delta > 180) delta -= 360;
            return delta;
        }
        
        /// <summary>
        /// Returns parameter t that produces linear interpolant for a and b
        /// </summary>
        public static float InverseLerp(float a, float b, float v)
        {
            return (v - a) / (b - a);
        }

        /// <summary>
        /// Returns the largest integral value less than or equal to the specified number
        /// </summary>
        public static float Floor(float a)
        {
            return (float)System.Math.Floor(a);
        }
        
        /// <summary>
        /// Linearly interpolates from a to b
        /// </summary>
        public static float Lerp(float a, float b, float t)
        {
            return a + t * (b - a);
        }
        
        /// <summary>
        /// Linearly interpolates vectors from a to b
        /// </summary>
        public static Vector2 Lerp(Vector2 a, Vector2 b, float t)
        {
            return new Vector2(Lerp(a.X, b.X, t), Lerp(a.Y, b.Y, t));
        }

        /// <summary>
        /// Linearly interpolates vectors from a to b
        /// </summary>
        public static Vector3 Lerp(Vector3 a, Vector3 b, float t)
        {
            return new Vector3(Lerp(a.X, b.X, t), Lerp(a.Y, b.Y, t), Lerp(a.Z, b.Z, t));
        }
        
        /// <summary>
        /// Linearly interpolates from angle a to angle b
        /// </summary>
        public static float LerpAngle(float a, float b, float t)
        {
            t = Clamp01(t);
            
            float cos = (1 - t) * Cos(a) + t * Cos(b);
            float sin = (1 - t) * Sin(a) + t * Sin(b);
            
            return Atan2(sin, cos);
        }

        /// <summary>
        /// Minimum value between a and b
        /// </summary>
        public static float Min(float a, float b)
        {
            return a <= b ? a : b;
        }

        /// <summary>
        /// Minimum value between a and b
        /// </summary>
        public static int Min(int a, int b)
        {
            return a <= b ? a : b;
        }

        /// <summary>
        /// Maximum value between a and b
        /// </summary>
        public static float Max(float a, float b)
        {
            return a <= b ? b : a;
        }

        /// <summary>
        /// Maximum value between a and b
        /// </summary>
        public static int Max(int a, int b)
        {
            return a <= b ? b : a;
        }
        
        /// <summary>
        /// Returns a raised to power of p
        /// </summary>
        public static float Pow(float a, float p)
        {
            return (float)System.Math.Pow(a, p);
        }

        /// <summary>
        /// Returns closest integer number
        /// </summary>
        public static float Round(float a)
        {
            return (float)System.Math.Round(a);
        }

        /// <summary>
        /// Returns 1 when value is positive or zero and -1 otherwise
        /// </summary>
        public static float Sign(float value)
        {
            return value >= 0 ? 1 : -1;
        }

        /// <summary>
        /// Sine of angle in radians
        /// </summary>
        public static float Sin(float angle)
        {
            return (float)System.Math.Sin(angle);
        }
        
        /// <summary>
        /// Returns square root
        /// </summary>
        public static float Sqrt(float a)
        {
            return (float)System.Math.Sqrt(a);
        }

        /// <summary>
        /// Tangent of angle in radians
        /// </summary>
        public static float Tan(float angle)
        {
            return (float)System.Math.Tan(angle);
        }
    }
}