#pragma once

#include <Iron.h>

class EditorApplication : public Application
{
public:
    explicit EditorApplication(ApplicationSettings settings);

    void RunUpdate() override;

    bool IsPlaying = true;
    int ApplicationX = 0;
    int ApplicationY = 0;
    int ApplicationWidth = 0;
    int ApplicationHeight = 0;
    int ApplicationWindowWidth = 0;
    int ApplicationWindowHeight = 0;
    Framebuffer* ApplicationFramebuffer = nullptr;
    ApplicationContext* EditorContext = nullptr;

private:
    void Init();
    void Terminate() override;
};
