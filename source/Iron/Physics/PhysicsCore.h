#pragma once

#include <box2d/box2d.h>

class PhysicsCore
{
public:
    static void Init();
    static void Terminate();

    static b2World* GetWorld();

    static void Step(float deltaTime);
};
