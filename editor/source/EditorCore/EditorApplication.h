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

    EditorStates::EditorState GetState() const;
    void SetState(EditorStates::EditorState newState);
    ApplicationContext* GetAppContext();
    Framebuffer* ApplicationFramebuffer = nullptr;
    ApplicationContext* EditorContext = nullptr;

private:
    void Terminate() override;

    EditorStates::EditorState state = EditorStates::Stopped;

    friend class AppView;
    friend class ControlPanel;
    friend class HierarchyView;
};
