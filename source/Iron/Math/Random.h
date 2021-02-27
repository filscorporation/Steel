#pragma once

#include <random>

class Random
{
public:
    static void Init();

    static uint64_t NextUint();

private:
    static std::mt19937 randomEngine;
};
