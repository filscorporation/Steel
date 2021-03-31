#pragma once

#include "../EntityComponentSystem/Entity.h"
#include "../UI/UIElements/UIText.h"

class DebugInfoWindow
{
public:
    DebugInfoWindow();

    void Enable() const;
    void Disable() const;
    void UpdateInfo();

private:
    void Create();

    int GetFPS();
    float lastFPS = 0.0f;

    static void ChangeWireframeMode();
    static void ChangeCameraMode();

    EntityID debugWindowEntity;
    EntityID fpsCounterEntity;
    EntityID drawCallsCounterEntity;
    EntityID verticesCounterEntity;
    // TODO: static until solution for buttons callbacks
    static EntityID wireframeModeStatusEntity;
    static EntityID freeCameraModeStatusEntity;
};
