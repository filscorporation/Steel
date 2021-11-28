#pragma once

#include "Steel/Rendering/Core/DrawCallsSource.h"
#include "Steel/Scene/Scene.h"

class UIDrawCallsSource : public DrawCallsSource
{
public:
    explicit UIDrawCallsSource(Scene* scene);

    void CollectDrawCalls(RenderContext* renderContext) override;

private:
    Scene* _scene;
};
