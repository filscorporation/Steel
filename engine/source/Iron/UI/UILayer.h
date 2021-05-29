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

    bool IsPointerOverUI();
    static UILayer* Current();

    void AddButtonToUpdateQueue(EntityID buttonID);
    void RemoveButtonFromUpdateQueue(EntityID buttonID);

    EntityID CreateUIElement();
    EntityID CreateUIElement(const char* name, EntityID parent);
    EntityID CreateUIImage();
    EntityID CreateUIImage(Sprite* sprite, const char* name, EntityID parent);
    EntityID CreateUIButton();
    EntityID CreateUIButton(Sprite* sprite, const char* name, EntityID parent);
    EntityID CreateUIText();
    EntityID CreateUIText(const char* text, const char* name, EntityID parent);

    int PixelsPerUnit = 32;

private:
    Scene* _scene;
    bool _isPointerOverUI = false;

    SparseSet _buttonsToUpdate;

    UISystem* uiSystem;
};
