#pragma once

#include "../EntityComponentSystem/ComponentSystem.h"
#include "UIElements/UIText.h"
#include "UIElements/UIImage.h"
#include "UIElements/UIInputField.h"
#include "UIElements/UIClipping.h"
#include "UIElements/UIButton.h"

class UISystem : public ComponentSystem<UIImage>, public ComponentSystem<UIButton>, public ComponentSystem<UIText>,
                 public ComponentSystem<UIInputField>, public ComponentSystem<UIClipping>
{
public:
    void OnComponentAdded(EntityID entityID, UIImage& component) override;
    void OnComponentRemoved(EntityID entityID, UIImage& component) override;
    void OnEntityEnabled(EntityID entityID, UIImage& component) override;
    void OnEntityDisabled(EntityID entityID, UIImage& component) override;

    void OnComponentAdded(EntityID entityID, UIButton& component) override;
    void OnComponentRemoved(EntityID entityID, UIButton& component) override;
    void OnEntityEnabled(EntityID entityID, UIButton& component) override;
    void OnEntityDisabled(EntityID entityID, UIButton& component) override;

    void OnComponentAdded(EntityID entityID, UIText& component) override;
    void OnComponentRemoved(EntityID entityID, UIText& component) override;
    void OnEntityEnabled(EntityID entityID, UIText& component) override;
    void OnEntityDisabled(EntityID entityID, UIText& component) override;

    void OnComponentAdded(EntityID entityID, UIInputField& component) override;
    void OnComponentRemoved(EntityID entityID, UIInputField& component) override;
    void OnEntityEnabled(EntityID entityID, UIInputField& component) override;
    void OnEntityDisabled(EntityID entityID, UIInputField& component) override;

    void OnComponentAdded(EntityID entityID, UIClipping& component) override;
    void OnComponentRemoved(EntityID entityID, UIClipping& component) override;
    void OnEntityEnabled(EntityID entityID, UIClipping& component) override;
    void OnEntityDisabled(EntityID entityID, UIClipping& component) override;

private:
    static bool CheckRectTransformation(EntitiesRegistry* entitiesRegistry, EntityID entityID);
    static void TryAddEventHandler(EntitiesRegistry* entitiesRegistry, EntityID entityID);
    void ClearRenderers(UIImage& image);
    void RenderersSetActive(const UIImage& image, bool active);
};
