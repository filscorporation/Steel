#pragma once

#include "../Scene/Scene.h"

class SceneRenderer
{
public:
    SceneRenderer(Scene* scene, Framebuffer* framebuffer, EntityID cameraEntity);

    virtual void DrawScene();

protected:
    void BeforeDraw();
    void AfterDraw();

    Scene* _scene;
    Framebuffer* _framebuffer;
    EntityID _cameraEntity;
};
