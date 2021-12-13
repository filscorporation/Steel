#pragma once

#include <Steel.h>

class EditorSceneRenderer
{
public:
    static void DrawToSceneView(Scene* scene, Framebuffer* framebuffer, EntityID cameraEntity);
};
