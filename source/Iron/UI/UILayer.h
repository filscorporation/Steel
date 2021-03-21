#pragma once

#include "../EntityComponentSystem/Entity.h"
#include "../Scene/Scene.h"
#include "UIEvent.h"

class Scene;

class UILayer
{
public:
    explicit UILayer(Scene* scene) { _scene = scene; }

    void Draw();
    void PollEvent(UIEvent& uiEvent);

    EntityID CreateUIElement();
    EntityID CreateUIElement(const char* name, EntityID parent);

    bool IsPointerOverUI();

private:
    Scene* _scene;
    bool _isPointerOverUI = false;
};
