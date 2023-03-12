#include "AndroidApplication.h"
#include "Steel/Rendering/Screen.h"
#include "Steel/Platform/FilesManager/AndroidFilesManager.h"
#include "Steel/Platform/Input/AndroidInputHandler.h"
#include "Steel/Platform/Rendering/EGLAPI.h"

struct UserData
{
    AndroidApplication* SteelApplication;
    struct android_app* AndroidApplication;
};

AndroidApplication::AndroidApplication(android_app* androidApp) : Application()
{
    Instance = this;
    _nativeApplication = androidApp;

    // Save applications as user data
    auto newData = new UserData();
    newData->SteelApplication = this;
    newData->AndroidApplication = androidApp;
    androidApp->userData = newData;
}

static int32_t AndroidHandleInput(struct android_app* androidApp, AInputEvent* event)
{
    AndroidInputHandler::HandleInputEvent(event);

    return 1;
}

static void AndroidHandleCMD(struct android_app* androidApplication, int32_t cmd)
{
    if (androidApplication->userData != nullptr)
    {
        auto userData = (UserData*)androidApplication->userData;
        userData->SteelApplication->HandleCMD(cmd);
    }
}

void AndroidApplication::RunEventLoop()
{
    // Setup handlers
    _nativeApplication->onAppCmd = AndroidHandleCMD;
    _nativeApplication->onInputEvent = AndroidHandleInput;

    do
    {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        while ((ident = ALooper_pollAll(0, nullptr, &events, (void**)&source)) >= 0)
        {
            if (source != nullptr)
                source->process(_nativeApplication, source);
        }

        if (IsInitialized)
        {
            // Call update every frame
            RunUpdate();
        }
    }
    while (!_nativeApplication->destroyRequested && (!IsInitialized || IsRunningInternal));
}

void AndroidApplication::Terminate()
{
    if (_nativeApplication->userData != nullptr)
    {
        delete (UserData*)_nativeApplication->userData;
        _nativeApplication->userData = nullptr;
    }

    // Terminate application
    Application::Terminate();
}

AndroidApplication* AndroidApplication::GetInstance()
{
    return (AndroidApplication*)Instance;
}

android_app* AndroidApplication::GetNativeApplication()
{
    return _nativeApplication;
}

void AndroidApplication::HandleCMD(int32_t cmd)
{
    switch (cmd)
    {
        case APP_CMD_INIT_WINDOW:
        {
            // TODO: can be sent when surface recreated

            // TODO: maybe here only init window and init app with files before (on creation)
            //Setup files manager
            AndroidFilesManager::CacheAllFiles(_nativeApplication->activity->assetManager);

            // Init application with default settings
            ApplicationSettings settings = ApplicationSettings();
            settings.DoubleBuffer = true;
            Init(settings);

            // Prepare to run in event loop
            PrepareRun();

            break;
        }
        case APP_CMD_TERM_WINDOW:
        {
            // TODO: can be sent when surface recreated

            // TODO: maybe here only terminate window
            IsRunningInternal = false;

            break;
        }
        case APP_CMD_WINDOW_RESIZED:
        {
            auto size = EGLAPI::GetDisplaySize();
            Screen::Resize(size.x, size.y);

            break;
        }
        default:
            break;
    }
}
