#pragma once

#include <Iron.h>

namespace EditorStates
{
    enum EditorState
    {
        Stopped,
        Playing,
        Paused,
        Step,
    };
}

class EditorApplication : public Application
{
public:
    EditorApplication() : Application() {}

    void Init(ApplicationSettings settings) override;
    void RunUpdate() override;

    EditorStates::EditorState State = EditorStates::Stopped;
    Framebuffer* ApplicationFramebuffer = nullptr;
    ApplicationContext* EditorContext = nullptr;

private:
    void Terminate() override;

    ApplicationContext* GetAppContext();

    friend class AppView;
};
