#include <iostream>
#include <GLAD/glad.h>

#include "Application.h"
#include "Log.h"
#include "Time.h"

bool isRunning;

Application* Application::Instance;
Scene* scene;

void Application::Init(ApplicationSettings settings)
{
    Screen::Init(settings.ScreenWidth, settings.ScreenHeight, settings.Fullscreen);

    gladLoadGL();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    scene = new Scene();

    Log::LogInfo("Application initialized");
}

Application::Application(ApplicationSettings settings)
{
    Init(settings);

    Instance = this;
}

void Application::Run()
{
    Log::LogInfo("Running application");

    isRunning = true;

    while (isRunning)
    {
        Input::PollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rendering objects in scene
        for (auto &object : scene->Objects)
        {
            for (auto &component : object->Components())
            {
                component->OnUpdate();
                component->OnRender(scene->MainCamera);
            }
        }

        Screen::SwapBuffers();

        Time::Update();

        if (Input::IsKeyPressed(Escape))
            isRunning = false;

        if (Screen::WindowShouldClose())
            isRunning = false;
    }
    Screen::Terminate();
}

Scene* Application::GetCurrentScene()
{
    return scene;
}