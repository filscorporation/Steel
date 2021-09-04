#pragma once

#include <Iron.h>

class EditorApplication : public Application
{
public:
    explicit EditorApplication(ApplicationSettings settings);

    void RunUpdate() override;

    bool IsPlaying = true;
    Framebuffer* ApplicationFramebuffer = nullptr;
    ApplicationContext* EditorContext = nullptr;

private:
    void Init();
    void Terminate() override;
};
