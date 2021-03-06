#pragma once

#include <cstdint>

class Entity;
class Camera;

class Component
{
public:
    explicit Component(Entity* parentEntity);
    virtual ~Component() { };

    uint64_t ID;

    Entity* ParentEntity = nullptr;

    virtual void OnUpdate() { };
};