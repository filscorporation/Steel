#pragma once

#include "ApplicationConfig.h"
#include "Steel/Scene/SceneManager.h"
#include "Steel/Scene/Scene.h"
#include "Steel/Resources/ResourcesManager.h"

struct ScreenParameters
{
    bool CanResize;
    bool Fullscreen;
    int Width, Height;
    int WidthBackup, HeightBackup;
    bool AutoResolution;
    int ResolutionX, ResolutionY;
    int OffsetX, OffsetY;
    glm::vec3 Color;
    bool IsDirty;
};

struct ApplicationSettings
{
    int ScreenWidth = 800;
    int ScreenHeight = 600;
    glm::vec3 ScreenColor = glm::vec3(0.2f, 0.3f, 0.3f);
    bool Fullscreen = false;
    bool DoubleBuffer = true;
    bool VSync = true;
    int DefaultPixelsPerUnit = 32;
};

struct ApplicationStats
{
    int DrawCalls = 0;
    int VerticesCount = 0;
};

struct ApplicationContext
{
    ResourcesManager* Resources = nullptr;
    ApplicationConfig* Config = nullptr;
    SceneManager* Scenes = nullptr;
    bool Scripting = true; // We do not support multiple script domains, but indicating context that is using it
    ScreenParameters ScreenParams {};
    ApplicationStats Stats {};
    bool IgnoreEvents = false;
};

class Application
{
public:
    static Application* Instance;
    static ApplicationContext* Context();

    Application();
    virtual void Init(ApplicationSettings settings);
    void PrepareRun();
    void RunMainLoop();
    virtual void Terminate();
    virtual void BeforeStartRunLoop();
    virtual void RunUpdate();
    void Quit();
    virtual bool IsRunning();

    const char* GetConfigPath();

    void SwitchScenes(Scene* newScene);
    ScreenParameters& ScreenParametersForUpdate();
    ResourcesManager* GetResourcesManager();
    SceneManager* GetSceneManager();
    Scene* GetCurrentScene();

    std::string GetRuntimePath();
    virtual std::string GetRelativeRootPath();
    std::string GetDataPath();

protected:
    bool IsInitialized = false;
    bool IsRunningInternal;
    ApplicationContext* CurrentContext = nullptr;
    ApplicationContext* AppContext = nullptr;
    Scene* SceneToSwitch = nullptr;
    std::string RootPath;

    void InitSystems(int width, int height, glm::vec3 color, bool fullscreen, bool doubleBuffer, bool vSync);
    ApplicationContext* CreateContext(ApplicationSettings settings);
    void TryLoadSceneOrCreateDefault(ApplicationContext* context);
    void SwitchContext(ApplicationContext* context);
    void TrySwitchScene();
};
