#pragma once

#include <Steel/EntityComponentSystem/Entity.h>
#include <Steel/UI/UIComponent.h>

#define FPS_FRAMES_COUNT 60
#define FPS_MAX_DELTA 0.03f

class StatsWindow : public UIComponent
{
    DEFINE_COMPONENT(StatsWindow)

public:
    explicit StatsWindow(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void Init();
    void UpdateInfo();

private:
    void UpdateFPS();
    int GetFPS();

    float frameTimer = 0.0f;
    float fpsTextUpdateTimer = 0.0f;
    float framesList[FPS_FRAMES_COUNT];
    int frameIndex = 0;

    EntityID debugWindowEntity;
    EntityID fpsCounterEntity;
    EntityID drawCallsCounterEntity;
    EntityID verticesCounterEntity;
};
