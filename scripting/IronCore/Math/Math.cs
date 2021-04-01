namespace Iron
{
    public static class Math
    {
        public static float Eps = 0.000001f;
        
        public static float Pi = 3.1415926536f;
        
        public static float DegToRad = Pi / 180f;
        
        public static float RadToDeg = 180f / Pi;

        public static float Abs(float a)
        {
            return a < 0 ? -a : a;
        }

        public static float Acos(float cos)
        {
            return (float)System.Math.Acos(cos);
        }

        public static float Asin(float sin)
        {
            return (float)System.Math.Asin(sin);
        }

        public static float Atan(float tan)
        {
            return (float)System.Math.Atan(tan);
        }

        public static float Atan2(float a, float b)
        {
            return (float)System.Math.Atan2(a, b);
        }
        
        public static bool Approximately(float a, float b)
        {
            return Abs(a - b) < Eps;
        }

        public static float Cos(float angle)
        {
            return (float)System.Math.Cos(angle);
        }
        
        public static float InverseLerp(float a, float b, float v)
        {
            return (v - a) / (b - a);
        }
        
        public static float Lerp(float a, float b, float t)
        {
            return a + t * (b - a);
        }

        public static Vector2 Lerp(Vector2 a, Vector2 b, float t)
        {
            return new Vector2(Lerp(a.X, b.X, t), Lerp(a.Y, b.Y, t));
        }

        public static Vector3 Lerp(Vector3 a, Vector3 b, float t)
        {
            return new Vector3(Lerp(a.X, b.X, t), Lerp(a.Y, b.Y, t), Lerp(a.Z, b.Z, t));
        }
        
        public static float Pow(float a, float p)
        {
            return (float)System.Math.Pow(a, p);
        }

        public static float Sin(float angle)
        {
            return (float)System.Math.Sin(angle);
        }
        
        public static float Sqrt(float a)
        {
            return (float)System.Math.Sqrt(a);
        }

        public static float Tan(float angle)
        {
            return (float)System.Math.Tan(angle);
        }
    }
}