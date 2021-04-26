#include <iostream>
#include "Random.h"

std::mt19937 Random::randomEngine;

void Random::Init()
{
    randomEngine.seed(std::random_device{}());
}

uint64_t Random::NextUint()
{
    std::uniform_int_distribution<uint64_t> dist;
    return dist(randomEngine);
}

float Random::NextFloat()
{
    std::uniform_real_distribution<float> dist;
    return dist(randomEngine);
}

float Random::NextFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(randomEngine);
}

int Random::NextInt()
{
    std::uniform_int_distribution<int> dist;
    return dist(randomEngine);
}

int Random::NextInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(randomEngine);
}
