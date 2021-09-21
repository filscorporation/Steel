#pragma once

#include <random>

class Random
{
public:
    static void Init();

    static uint64_t NextUint();
    static float NextFloat();
    static float NextFloat(float min, float max);
    static int NextInt();
    static int NextInt(int min, int max);

private:
    static std::mt19937 randomEngine;
};
