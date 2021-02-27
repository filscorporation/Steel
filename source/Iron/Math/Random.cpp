#include "Random.h"

std::mt19937 Random::randomEngine;

void Random::Init()
{
    randomEngine.seed(std::random_device()());
}

uint64_t Random::NextUint()
{
    std::uniform_int_distribution<uint64_t> dist;
    return dist(randomEngine);
}
