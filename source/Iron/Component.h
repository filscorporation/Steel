#pragma once

class Object;

class Component
{
public:
    Object* ParentObject = nullptr;

    virtual void OnUpdate() { };
    virtual void OnRender() { };
};