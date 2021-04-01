#include "Debug.h"
#include "../Core/Application.h"
#include "../Core/Input.h"
#include "../Core/Time.h"

DebugInfoWindow* Debug::debugInfoWindow;

bool Debug::FreeCameraMode = false;

void Debug::Init()
{
#ifdef DEBUG_BUILD
    debugInfoWindow = new DebugInfoWindow();
#endif
}

void Debug::Terminate()
{
#ifdef DEBUG_BUILD
    delete debugInfoWindow;
#endif
}

void Debug::EnableDebugWindow()
{
    debugInfoWindow->Enable();
}

void Debug::DisableDebugWindow()
{
    debugInfoWindow->Disable();
}

void Debug::Update()
{
#ifdef DEBUG_BUILD
    debugInfoWindow->UpdateInfo();

    if (FreeCameraMode)
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        auto& mainCamera = Application::Instance->GetCurrentScene()->GetMainCamera();
        auto& mainCameraTransformation = registry->GetComponent<Transformation>(mainCamera.Owner);

        float speed = 0.25f * Time::DeltaTime() * mainCamera.GetWidth();
        if (Input::IsKeyPressed(KeyCodes::A))
        {
            auto pos = mainCameraTransformation.GetLocalPosition();
            mainCameraTransformation.SetLocalPosition(pos + glm::vec3(-speed, 0, 0));
        }
        if (Input::IsKeyPressed(KeyCodes::D))
        {
            auto pos = mainCameraTransformation.GetLocalPosition();
            mainCameraTransformation.SetLocalPosition(pos + glm::vec3(speed, 0, 0));
        }
        if (Input::IsKeyPressed(KeyCodes::W))
        {
            auto pos = mainCameraTransformation.GetLocalPosition();
            mainCameraTransformation.SetLocalPosition(pos + glm::vec3(0, speed, 0));
        }
        if (Input::IsKeyPressed(KeyCodes::S))
        {
            auto pos = mainCameraTransformation.GetLocalPosition();
            mainCameraTransformation.SetLocalPosition(pos + glm::vec3(0, -speed, 0));
        }
        if (Input::GetMouseScrollDelta().y != 0.0f)
        {
            mainCamera.SetWidth(mainCamera.GetWidth() - Input::GetMouseScrollDelta().y * 0.2f);
        }
    }
#endif
}

void Debug::PauseUpdate()
{
    // not implemented yet
}

void Debug::ContinueUpdate()
{
    // not implemented yet
}
