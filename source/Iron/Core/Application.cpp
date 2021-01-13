#include <iostream>
#include <GLAD/glad.h>

#include "Application.h"
#include "Log.h"
#include "Time.h"
#include "../Rendering/Renderer.h"

bool isRunning;

Application* Application::Instance;
ResourcesManager* resources;
Scene* scene;

Application::Application(ApplicationSettings settings)
{
    Init(settings);

    Instance = this;
}

void Application::Init(ApplicationSettings settings)
{
    Screen::Init(settings.ScreenWidth, settings.ScreenHeight, settings.Fullscreen);

    gladLoadGL();

    resources = new ResourcesManager();
    scene = new Scene();

    Renderer::Init(scene->MainCamera);

    Log::LogInfo("Application initialized");
}

void Application::Run()
{
    Log::LogInfo("Running application");

    isRunning = true;

    while (isRunning)
    {
        Input::PollEvents();

        Renderer::Clear(glm::vec3(0.2f, 0.3f, 0.3f));
        Renderer::OnBeforeRender();

        // Update and render objects in scene
        for (auto &object : scene->Objects)
        {
            for (auto &component : object->Components())
            {
                component->OnUpdate();
            }
        }
        for (auto &object : scene->Objects)
        {
            for (auto &component : object->Components())
            {
                component->OnLateUpdate();
            }
        }
        for (auto &object : scene->Objects)
        {
            for (auto &component : object->Components())
            {
                component->OnRender();
            }
        }

        Screen::SwapBuffers();

        Time::Update();

        if (Input::IsKeyPressed(Escape))
            isRunning = false;

        if (Screen::WindowShouldClose())
            isRunning = false;
    }
    Renderer::Terminate();
    Screen::Terminate();
}

ResourcesManager *Application::GetResourcesManager()
{
    return resources;
}


Scene* Application::GetCurrentScene()
{
    return scene;
}