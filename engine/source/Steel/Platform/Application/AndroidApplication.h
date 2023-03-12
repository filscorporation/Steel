#pragma once

#include "Steel/Core/Application.h"

#include <android_native_app_glue.h>

class AndroidApplication : public Application
{
public:
    AndroidApplication(struct android_app* androidApp);

    void RunEventLoop();
    void Terminate() override;

    void HandleCMD(int32_t cmd);

    static AndroidApplication* GetInstance();
    struct android_app* GetNativeApplication();

private:
    android_app* _nativeApplication = nullptr;
};
