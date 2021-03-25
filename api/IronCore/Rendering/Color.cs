namespace Iron
{
    public struct Color
    {
        private const int CHANNEL_MAX = 255;
        
        public float R { get; }
        public float G { get; }
        public float B { get; }
        public float A { get; }

        public Color(float r, float g, float b, float a)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }

        public Color(float r, float g, float b)
        {
            R = r;
            G = g;
            B = b;
            A = 1.0f;
        }

        public Color(int r, int g, int b, float a)
        {
            R = (float)r / CHANNEL_MAX;
            G = (float)g / CHANNEL_MAX;
            B = (float)b / CHANNEL_MAX;
            A = a;
        }

        public Color(int r, int g, int b)
        {
            R = (float)r / CHANNEL_MAX;
            G = (float)g / CHANNEL_MAX;
            B = (float)b / CHANNEL_MAX;
            A = 1.0f;
        }
        
        public static bool operator ==(Color a, Color b) => Equals(a, b);
        public static bool operator !=(Color a, Color b) => !Equals(a, b);

        public override bool Equals(object obj)
        {
            if (obj is Color col)
                return Math.Approximately(R, col.R)
                       && Math.Approximately(G, col.G)
                       && Math.Approximately(B, col.B)
                       && Math.Approximately(A, col.A);
            
            return base.Equals(obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                int result = 0;
                result = (result * 397) ^ R.GetHashCode();
                result = (result * 397) ^ G.GetHashCode();
                result = (result * 397) ^ B.GetHashCode();
                result = (result * 397) ^ A.GetHashCode();
                
                return result;
            }
        }

        public static Color Black => new Color(0, 0, 0);
        public static Color White => new Color(255, 255, 255);
        public static Color Red => new Color(255, 0, 0);
        public static Color Green => new Color(0, 255, 0);
        public static Color Blue => new Color(0, 0, 255);
        public static Color Yellow => new Color(255, 255, 0);
        public static Color Cyan => new Color(0, 255, 255);
        public static Color Magenta => new Color(255, 0, 255);
        public static Color Gray => new Color(192, 192, 192);
        public static Color DarkGray => new Color(128, 128, 128);
        public static Color DarkRed => new Color(128, 0, 0);
        public static Color DarkGreen => new Color(0, 128, 0);
        public static Color Purple => new Color(128, 0, 128);
        public static Color Teal => new Color(0, 128, 128);
        public static Color DarkBlue => new Color(0, 0, 128);
        public static Color Brown => new Color(139, 69, 19);
        public static Color Orange => new Color(255, 165, 0);
        public static Color Pink => new Color(255, 105, 180);
    }
}