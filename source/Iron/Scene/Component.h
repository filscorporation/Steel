#pragma once

class Object;
class Camera;

class Component
{
public:
    virtual ~Component() { };

    Object* ParentObject = nullptr;

    virtual void OnUpdate() { };
    virtual void OnLateUpdate() { };
    virtual void OnRender() { };
};