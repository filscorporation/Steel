#pragma once

#include "Steel/Rendering/Core/DrawCallsSource.h"
#include "Camera.h"
#include "Steel/Scene/Scene.h"

class SceneDrawCallsSource : public DrawCallsSource
{
public:
    SceneDrawCallsSource(Scene* scene, RenderMask mask);

    void CollectDrawCalls(RenderContext* renderContext) override;

private:
    Scene* _scene;
    RenderMask _mask;
};
