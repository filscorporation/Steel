#pragma once

#include "../EntityComponentSystem/SparseSet.h"
#include "../Scene/Scene.h"
#include "UIEvent.h"
#include "UIInteractable.h"
#include "UIElements/LayoutGroupTypes.h"

class Scene;

class UILayer
{
public:
    explicit UILayer(Scene* scene);

    void Update();
    void Rebuild();
    void Draw();
    void Refresh();

    void LoadDefaultResources();
    void PollEvent(UIEvent& uiEvent);

    bool IsPointerOverUI() const;
    static UILayer* Current();
    uint32_t GetLayerThickness() const;
    uint32_t GetCurrentHierarchyIndex() const;
    void IncreaseCurrentHierarchyIndex(uint32_t thickness);
    void SetSortingOrderDirty();
    bool NeedRebuildSortingOrder() const;

    void AddToUpdateQueue(EntityID entityID, UpdateInteractable callback);
    void RemoveFromUpdateQueue(EntityID entityID);

    EntityID CreateUIElement();
    EntityID CreateUIElement(const char* name, EntityID parent);
    EntityID CreateUIImage();
    EntityID CreateUIImage(Sprite* sprite, const char* name, EntityID parent);
    EntityID CreateUIButton();
    EntityID CreateUIButton(Sprite* sprite, const char* name, EntityID parent);
    EntityID CreateUIText();
    EntityID CreateUIText(const char* text, const char* name, EntityID parent);
    EntityID CreateUIInputField();
    EntityID CreateUIInputField(const char* name, EntityID parent);
    EntityID CreateUIClipping();
    EntityID CreateUIClipping(EntityID parent);
    EntityID CreateUICheckBox();
    EntityID CreateUICheckBox(const char* label, const char* name, EntityID parent);
    EntityID CreateUITabs();
    EntityID CreateUITabs(const std::vector<std::string>& tabsNames, const char* name, EntityID parent);
    EntityID CreateUILayoutGroup(LayoutGroupTypes::LayoutGroupType type);
    EntityID CreateUILayoutGroup(LayoutGroupTypes::LayoutGroupType type, const char* name, EntityID parent);

    int PixelsPerUnit = 32;

    struct UIResourcesStorage
    {
        Sprite* DefaultFrameSprite = nullptr;
        Sprite* DefaultPixelSprite = nullptr;
        Sprite* DefaultCheckBoxSprite = nullptr;
        Sprite* DefaultCheckMarkSprite = nullptr;
        Sprite* DefaultTabOpenedSprite = nullptr;
        Sprite* DefaultTabClosedSprite = nullptr;
    };
    UIResourcesStorage UIResources;

private:
    Scene* _scene;
    bool _isPointerOverUI = false;

    uint32_t _layerThickness = 0;
    uint32_t _currentHierarchyIndex = 0;
    bool _rebuildSortingOrder = true;

    struct InteractableCallback
    {
        InteractableCallback(UpdateInteractable callback, EntityID owner)
        {
            Callback = callback;
            Owner = owner;
        }
        UpdateInteractable Callback;
        EntityID Owner;
    };
    SparseDataSet<InteractableCallback> _updateQueue;
};
