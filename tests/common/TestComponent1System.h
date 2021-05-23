#pragma once

#include "Iron/EntityComponentSystem/ComponentSystem.h"
#include "TestComponent1.h"

class TestComponent1System : public ComponentSystem<TestComponent1>
{
public:
    void OnComponentAdded(EntityID entityID, TestComponent1& component) override;
    void OnComponentRemoved(EntityID entityID, TestComponent1& component) override;
    void OnEntityEnabled(EntityID entityID, TestComponent1& component) override;
    void OnEntityDisabled(EntityID entityID, TestComponent1& component) override;

    int CatchAdded = 0;
    int CatchRemoved = 0;
    int CatchEnabled = 0;
    int CatchDisabled = 0;
};
