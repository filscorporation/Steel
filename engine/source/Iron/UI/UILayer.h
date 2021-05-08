#pragma once

#include "../EntityComponentSystem/SparseSet.h"
#include "../Scene/Scene.h"
#include "UIEvent.h"
#include "UIElements/UIButton.h"
#include "UISystem.h"

class Scene;

class UILayer
{
public:
    explicit UILayer(Scene* scene);
    ~UILayer();

    void Update();
    void Draw();
    void PollEvent(UIEvent& uiEvent);

    EntityID CreateUIElement();
    EntityID CreateUIElement(const char* name, EntityID parent);

    void AddButtonToUpdateQueue(EntityID buttonID);
    void RemoveButtonFromUpdateQueue(EntityID buttonID);

    bool IsPointerOverUI();
    static UILayer* Current();

    int PixelsPerUnit = 32;

private:
    Scene* _scene;
    bool _isPointerOverUI = false;

    SparseSet _buttonsToUpdate;

    UISystem* uiSystem;
};
