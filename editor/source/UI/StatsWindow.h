#pragma once

#include <Iron/EntityComponentSystem/Entity.h>
#include <Iron/UI/UIComponent.h>

class StatsWindow : public UIComponent
{
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
