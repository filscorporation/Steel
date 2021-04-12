#pragma once

#include "../EntityComponentSystem/ComponentSystem.h"
#include "UIElements/UIText.h"
#include "UIElements/UIImage.h"
#include "UIElements/UIButton.h"

class UISystem : public ComponentSystem<UIText>, public ComponentSystem<UIImage>, public ComponentSystem<UIButton>
{
public:
    void OnComponentAdded(EntityID entityID, UIText& component) override;
    void OnComponentRemoved(EntityID entityID, UIText& component) override;
    void OnEntityEnabled(EntityID entityID, UIText& component) override;
    void OnEntityDisabled(EntityID entityID, UIText& component) override;

    void OnComponentAdded(EntityID entityID, UIImage& component) override;
    void OnComponentRemoved(EntityID entityID, UIImage& component) override;
    void OnEntityEnabled(EntityID entityID, UIImage& component) override;
    void OnEntityDisabled(EntityID entityID, UIImage& component) override;

    void OnComponentAdded(EntityID entityID, UIButton& component) override;
    void OnComponentRemoved(EntityID entityID, UIButton& component) override;
    void OnEntityEnabled(EntityID entityID, UIButton& component) override;
    void OnEntityDisabled(EntityID entityID, UIButton& component) override;

private:
    static bool CheckRectTransformation(EntitiesRegistry* entitiesRegistry, EntityID entityID);
    static void TryAddUIRenderer(EntitiesRegistry* entitiesRegistry, EntityID entityID);
    static void TryAddEventHandler(EntitiesRegistry* entitiesRegistry, EntityID entityID);
};
