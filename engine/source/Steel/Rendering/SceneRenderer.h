#pragma once

class Scene;
class Framebuffer;

class SceneRenderer
{
public:
    static void Draw(Scene* scene, Framebuffer* framebuffer);
};
