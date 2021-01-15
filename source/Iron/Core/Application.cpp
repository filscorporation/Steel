#include <iostream>
#include <GLAD/glad.h>

#include "Application.h"
#include "Log.h"
#include "Time.h"
#include "../Rendering/Renderer.h"

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
    Screen::Init(settings.ScreenWidth, settings.ScreenHeight, settings.ScreenColor, settings.Fullscreen);

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

    float timer = 0;
    int counter = 0;

    while (isRunning)
    {
        Input::PollEvents();

        Renderer::Clear(Screen::Color());
        Renderer::OnBeforeRender();

        // Update and render objects in scene
        // TODO: optimize copying (its needed if objects added in updates)
        auto objects = new std::vector<Object*>(scene->Objects);
        for (auto &object : *objects)
        {
            for (auto &component : object->Components())
            {
                try
                {
                    component->OnUpdate();
                }
                catch (const std::exception& ex)
                {
                    Log::LogError("Error in update: " + std::string(ex.what()));
                }
            }
        }
        for (auto &object : *objects)
        {
            for (auto &component : object->Components())
            {
                try
                {
                    component->OnLateUpdate();
                }
                catch (const std::exception& ex)
                {
                    Log::LogError("Error in late update: " + std::string(ex.what()));
                }
            }
        }
        for (auto &object : *objects)
        {
            for (auto &component : object->Components())
            {
                try
                {
                    // TODO: change to render calls and do depth sorting
                    component->OnRender();
                }
                catch (const std::exception& ex)
                {
                    Log::LogError("Error in render: " + std::string(ex.what()));
                }
            }
        }
        //Freeing copy
        free(objects);

        Screen::SwapBuffers();

        Time::Update();

        if (Screen::WindowShouldClose())
            isRunning = false;

        // TODO: testing fps
        counter ++;
        timer += Time::DeltaTime();
        if (timer > 1.0f)
        {
            Log::LogInfo(std::to_string(counter));
            timer = 0;
            counter = 0;
        }
    }
    Renderer::Terminate();
    Screen::Terminate();
}

void Application::Quit()
{
    isRunning = false;
}

ResourcesManager *Application::GetResourcesManager()
{
    return resources;
}

Scene* Application::GetCurrentScene()
{
    return scene;
}
