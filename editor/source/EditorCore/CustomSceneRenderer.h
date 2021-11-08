#pragma once

#include <Steel/Rendering/SceneRenderer.h>

class CustomSceneRenderer : public SceneRenderer
{
public:
    CustomSceneRenderer(Scene* scene, Framebuffer* framebuffer, EntityID cameraEntity)
        : SceneRenderer(scene, framebuffer, cameraEntity) { }

    void DrawScene() override;

private:
    void RenderPass(Camera& camera);
};
