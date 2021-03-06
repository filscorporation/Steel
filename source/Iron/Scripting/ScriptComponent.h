#pragma once

#include "../Scene/Component.h"
#include "../Physics/Collision.h"

struct ScriptMethods;
struct Collision;

class ScriptComponent : public Component
{
public:
    explicit ScriptComponent(Entity* parentEntity) : Component(parentEntity) { }

    void Init(const char* fullName, int64_t scriptPointer);

    void OnUpdate() override;

    void OnCreate();
    void OnFixedUpdate();
    void OnLateUpdate();

    void OnCollisionEnter(Collision collision);
    void OnCollisionExit(Collision collision);

    void OnMouseOver();
    void OnMouseEnter();
    void OnMouseExit();
    void OnMousePressed();
    void OnMouseJustPressed();
    void OnMouseJustReleased();

private:
    ScriptMethods* methods;
    const char* _fullName;
    int64_t _scriptPointer;

    bool hasCollision = false;
    Collision collisionStay;
};
