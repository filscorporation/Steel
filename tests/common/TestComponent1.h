#pragma once

#include "Steel/EntityComponentSystem/Component.h"

class TestComponent1 : public Component
{
    COMPONENT(TestComponent1)

public:
    explicit TestComponent1(EntityID ownerEntityID) : Component(ownerEntityID) { }

    void OnCreated(EntitiesRegistry* entitiesRegistry) override;
    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;
    void OnDisabled(EntitiesRegistry* entitiesRegistry) override;

    int IntegerValue = 0;
};
