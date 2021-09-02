#pragma once

#include "SceneManager.h"
#include "../Scene/Scene.h"
#include "../Resources/ResourcesManager.h"

struct ApplicationSettings
{
    int ScreenWidth = 800;
    int ScreenHeight = 600;
    glm::vec3 ScreenColor = glm::vec3(0.2f, 0.3f, 0.3f);
    bool Fullscreen = false;
    bool DoubleBuffer = false;
};

class Application
{
public:
    static Application* Instance;

    explicit Application(ApplicationSettings settings);
    void Run();
    void RunUpdate();
    void Quit();
    ResourcesManager* GetResourcesManager();
    SceneManager* GetSceneManager();
    Scene* GetCurrentScene();
    std::string GetRuntimePath();
    std::string GetDataPath();

private:
    bool isRunning;
    ResourcesManager* resourcesManager;
    SceneManager* sceneManager;

    void Init(ApplicationSettings settings);
    void Terminate();
};
