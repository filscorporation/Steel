#pragma once

#include "../EntityComponentSystem/Entity.h"
#include "../Scene/Scene.h"

class Scene;

class UILayer
{
public:
    explicit UILayer(Scene* scene) { _scene = scene; }

    void Draw();

    EntityID CreateUIElement();
    EntityID CreateUIElement(const char* name, EntityID parent);

private:
    Scene* _scene;
};
