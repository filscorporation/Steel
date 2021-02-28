namespace Iron.Math
{
    public static class Math
    {
        public static float Eps = 0.000001f;

        public static float Abs(float a)
        {
            return a < 0 ? -a : a;
        }
        
        public static bool Approximately(float a, float b)
        {
            return Abs(a - b) < Eps;
        }
    }
}