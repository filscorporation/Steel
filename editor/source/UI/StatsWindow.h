#pragma once

#include <Steel/EntityComponentSystem/Entity.h>
#include <Steel/UI/UIComponent.h>

class StatsWindow : public UIComponent
{
    COMPONENT(StatsWindow)

public:
    explicit StatsWindow(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void Init();
    void UpdateInfo();

private:
    int GetFPS();
    float lastFPS = 0.0f;

    EntityID debugWindowEntity;
    EntityID fpsCounterEntity;
    EntityID drawCallsCounterEntity;
    EntityID verticesCounterEntity;
};
