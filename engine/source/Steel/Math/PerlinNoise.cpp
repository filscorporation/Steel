#include "PerlinNoise.h"
#include "Math.h"

#include <cmath>
#include <cstdlib>

#define MAX_PRIME_NUMBER 10

int numOctaves = 7;
double persistence = 0.5;
int primeIndex = 0;

int primes[MAX_PRIME_NUMBER][3] = {
        { 995615039, 600173719, 701464987 },
        { 831731269, 162318869, 136250887 },
        { 174329291, 946737083, 245679977 },
        { 362489573, 795918041, 350777237 },
        { 457025711, 880830799, 909678923 },
        { 787070341, 177340217, 593320781 },
        { 405493717, 291031019, 391950901 },
        { 458904767, 676625681, 424452397 },
        { 531736441, 939683957, 810651871 },
        { 997169939, 842027887, 423882827 }
};

double Noise(int i, int x, int y)
{
    int n = x + y * 57;
    n = (n << 13) ^ n;
    int a = primes[i][0], b = primes[i][1], c = primes[i][2];
    int t = (n * (n * n * a + b) + c) & 0x7fffffff;

    return 1.0 - (double)(t)/1073741824.0;
}

double SmoothedNoise(int i, int x, int y)
{
    double corners = (Noise(i, x-1, y-1) + Noise(i, x+1, y-1) +
                      Noise(i, x-1, y+1) + Noise(i, x+1, y+1)) / 16,
            sides = (Noise(i, x-1, y) + Noise(i, x+1, y) + Noise(i, x, y-1) +
                     Noise(i, x, y+1)) / 8,
            center = Noise(i, x, y) / 4;

    return corners + sides + center;
}

double Interpolate(double a, double b, double x)
{
    double ft = x * Math::Pi, f = (1 - std::cos(ft)) * 0.5;
    return a * (1 - f) + b * f;
}

double InterpolatedNoise(int i, double x, double y)
{
    int integerX = (int)x;
    double fractionalX = x - integerX;
    int integerY = (int)y;
    double fractionalY = y - integerY;

    double v1 = SmoothedNoise(i, integerX, integerY);
    double v2 = SmoothedNoise(i, integerX + 1, integerY);
    double v3 = SmoothedNoise(i, integerX, integerY + 1);
    double v4 = SmoothedNoise(i, integerX + 1, integerY + 1);
    double i1 = Interpolate(v1, v2, fractionalX);
    double i2 = Interpolate(v3, v4, fractionalX);

    return Interpolate(i1, i2, fractionalY);
}

float PerlinNoise::Get(float x, float y)
{
    double total = 0, frequency = pow(2, numOctaves), amplitude = 1;
    for (int i = 0; i < numOctaves; ++i)
    {
        frequency /= 2;
        amplitude *= persistence;
        total += InterpolatedNoise((primeIndex + i) % MAX_PRIME_NUMBER, x / frequency, y / frequency) * amplitude;
    }

    return (float)(total / frequency);
}
