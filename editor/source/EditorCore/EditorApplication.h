#pragma once

#include <Steel.h>

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

    ApplicationContext* GetAppContext();

    EditorStates::EditorState State = EditorStates::Stopped;
    Framebuffer* ApplicationFramebuffer = nullptr;
    ApplicationContext* EditorContext = nullptr;

private:
    void Terminate() override;

    friend class AppView;
    friend class HierarchyView;
};
