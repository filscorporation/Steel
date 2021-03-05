#pragma once

#include <box2d/box2d.h>

#include "PhysicsContactListener.h"

class PhysicsCore
{
public:
    static void Init();
    static void Terminate();

    static b2World* GetWorld();

    static void Simulate(float deltaTime);
};
