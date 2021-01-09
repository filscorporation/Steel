#pragma once

#include "Object.h"
#include "Scene.h"

class Application
{
public:
    static Application* Instance;

    Application();
    void Run();
    Scene* GetCurrentScene();
};