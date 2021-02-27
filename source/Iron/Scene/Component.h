#pragma once

#include <cstdint>

class Entity;
class Camera;

class Component
{
public:
    Component();
    virtual ~Component() { };

    uint64_t ID;

    Entity* ParentEntity = nullptr;

    virtual void OnUpdate() { };
    virtual void OnLateUpdate() { };
    virtual void OnRender() { };
};