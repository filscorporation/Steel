#include "DebugInfoWindow.h"
#include "../Core/Time.h"
#include "../Rendering/Renderer.h"
#include "Debug.h"

EntityID DebugInfoWindow::wireframeModeStatusEntity = NULL_ENTITY;
EntityID DebugInfoWindow::freeCameraModeStatusEntity = NULL_ENTITY;

DebugInfoWindow::DebugInfoWindow()
{
    Create();
    Disable();
}

void DebugInfoWindow::Enable() const
{
    Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntitySetActive(debugWindowEntity, true, true);
}

void DebugInfoWindow::Disable() const
{
    Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->EntitySetActive(debugWindowEntity, false, true);
}

void DebugInfoWindow::UpdateInfo()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    registry->GetComponent<UIText>(fpsCounterEntity).SetText(std::to_wstring(GetFPS()));
    registry->GetComponent<UIText>(drawCallsCounterEntity).SetText(std::to_wstring(Renderer::DrawCallsStats));
    registry->GetComponent<UIText>(verticesCounterEntity).SetText(std::to_wstring(Renderer::VerticesStats));
}

void DebugInfoWindow::Create()
{
    auto resources = Application::Instance->GetResourcesManager();
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    auto windowSprite = resources->LoadImage("debug_panel.png", true);
    auto buttonSprite = resources->LoadImage("debug_button.png", true);

    {
        debugWindowEntity = scene->GetUILayer()->CreateUIElement("Debug window", NULL_ENTITY);
        auto& windowImage = registry->AddComponent<UIImage>(debugWindowEntity);
        windowImage.SetImage(windowSprite);
        auto& windowRT = registry->GetComponent<RectTransformation>(debugWindowEntity);
        windowRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        windowRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        windowRT.SetSize(glm::vec2(200, 114));
        windowRT.SetAnchoredPosition(-windowRT.GetSize() * 0.5f - glm::vec2(2, 2));
        windowRT.SetSortingOrder(-90);
    }

    float y = 6;

    {
        auto fpsLabelEntity = scene->GetUILayer()->CreateUIElement("FPS label", debugWindowEntity);
        auto& fpsLabelText = registry->AddComponent<UIText>(fpsLabelEntity);
        fpsLabelText.SetTextSize(16);
        fpsLabelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        fpsLabelText.SetText(L"FPS");
        auto& fpsLabelRT = registry->GetComponent<RectTransformation>(fpsLabelEntity);
        fpsLabelRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        fpsLabelRT.SetAnchorMax(glm::vec2(0.0f, 1.0f));
        fpsLabelRT.SetSize(glm::vec2(100, 20));
        fpsLabelRT.SetAnchoredPosition(glm::vec2(50 + 2, -y - 20 * 0.5f));
        fpsLabelRT.SetSortingOrder(-1);
    }
    {
        fpsCounterEntity = scene->GetUILayer()->CreateUIElement("FPS counter", debugWindowEntity);
        auto& fpsCounterText = registry->AddComponent<UIText>(fpsCounterEntity);
        fpsCounterText.SetTextSize(16);
        fpsCounterText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        fpsCounterText.SetText(L"0");
        auto& fpsCounterRT = registry->GetComponent<RectTransformation>(fpsCounterEntity);
        fpsCounterRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        fpsCounterRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        fpsCounterRT.SetSize(glm::vec2(100, 20));
        fpsCounterRT.SetAnchoredPosition(glm::vec2(-50 - 2, -y - 20 * 0.5f));
        fpsCounterRT.SetSortingOrder(-1);
    }

    y += 20;

    {
        auto drawCallsLabelEntity = scene->GetUILayer()->CreateUIElement("Draw Calls label", debugWindowEntity);
        auto& drawCallsLabelText = registry->AddComponent<UIText>(drawCallsLabelEntity);
        drawCallsLabelText.SetTextSize(16);
        drawCallsLabelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        drawCallsLabelText.SetText(L"Draw calls");
        auto& drawCallsLabelRT = registry->GetComponent<RectTransformation>(drawCallsLabelEntity);
        drawCallsLabelRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        drawCallsLabelRT.SetAnchorMax(glm::vec2(0.0f, 1.0f));
        drawCallsLabelRT.SetSize(glm::vec2(100, 20));
        drawCallsLabelRT.SetAnchoredPosition(glm::vec2(50 + 2, -y - 20 * 0.5f));
        drawCallsLabelRT.SetSortingOrder(-1);
    }
    {
        drawCallsCounterEntity = scene->GetUILayer()->CreateUIElement("Draw Calls counter", debugWindowEntity);
        auto& drawCallsCounterText = registry->AddComponent<UIText>(drawCallsCounterEntity);
        drawCallsCounterText.SetTextSize(16);
        drawCallsCounterText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        drawCallsCounterText.SetText(L"0");
        auto& drawCallsCounterRT = registry->GetComponent<RectTransformation>(drawCallsCounterEntity);
        drawCallsCounterRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        drawCallsCounterRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        drawCallsCounterRT.SetSize(glm::vec2(100, 20));
        drawCallsCounterRT.SetAnchoredPosition(glm::vec2(-50 - 2, -y - 20 * 0.5f));
        drawCallsCounterRT.SetSortingOrder(-1);
    }

    y += 20;

    {
        auto verticesLabelEntity = scene->GetUILayer()->CreateUIElement("Vertices label", debugWindowEntity);
        auto& verticesLabelText = registry->AddComponent<UIText>(verticesLabelEntity);
        verticesLabelText.SetTextSize(16);
        verticesLabelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        verticesLabelText.SetText(L"Vertices count");
        auto& verticesLabelRT = registry->GetComponent<RectTransformation>(verticesLabelEntity);
        verticesLabelRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        verticesLabelRT.SetAnchorMax(glm::vec2(0.0f, 1.0f));
        verticesLabelRT.SetSize(glm::vec2(100, 20));
        verticesLabelRT.SetAnchoredPosition(glm::vec2(50 + 2, -y - 20 * 0.5f));
        verticesLabelRT.SetSortingOrder(-1);
    }
    {
        verticesCounterEntity = scene->GetUILayer()->CreateUIElement("Vertices counter", debugWindowEntity);
        auto& verticesCounterText = registry->AddComponent<UIText>(verticesCounterEntity);
        verticesCounterText.SetTextSize(16);
        verticesCounterText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        verticesCounterText.SetText(L"0");
        auto& verticesCounterRT = registry->GetComponent<RectTransformation>(verticesCounterEntity);
        verticesCounterRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        verticesCounterRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        verticesCounterRT.SetSize(glm::vec2(100, 20));
        verticesCounterRT.SetAnchoredPosition(glm::vec2(-50 - 2, -y - 20 * 0.5f));
        verticesCounterRT.SetSortingOrder(-1);
    }

    y += 20;

    {
        auto labelEntity = scene->GetUILayer()->CreateUIElement("Wireframe Mode label", debugWindowEntity);
        auto& labelText = registry->AddComponent<UIText>(labelEntity);
        labelText.SetTextSize(16);
        labelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        labelText.SetText(L"Wireframe mode");
        auto& labelRT = registry->GetComponent<RectTransformation>(labelEntity);
        labelRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        labelRT.SetAnchorMax(glm::vec2(0.0f, 1.0f));
        labelRT.SetSize(glm::vec2(120, 20));
        labelRT.SetAnchoredPosition(glm::vec2(60 + 2, -y - 20 * 0.5f));
        labelRT.SetSortingOrder(-1);
    }
    {
        auto buttonEntity = scene->GetUILayer()->CreateUIElement("Wireframe Mode button", debugWindowEntity);
        auto& button = registry->AddComponent<UIButton>(buttonEntity);
        button.SetImage(buttonSprite);
        button.Callback = ChangeWireframeMode;
        auto& buttonRT = registry->GetComponent<RectTransformation>(buttonEntity);
        buttonRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        buttonRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        buttonRT.SetSize(glm::vec2(80, 20));
        buttonRT.SetAnchoredPosition(glm::vec2(-40 - 2, -y - 20 * 0.5f));
        buttonRT.SetSortingOrder(-1);

        wireframeModeStatusEntity = scene->GetUILayer()->CreateUIElement("Wireframe Mode status", buttonEntity);
        auto& labelText = registry->AddComponent<UIText>(wireframeModeStatusEntity);
        labelText.SetTextSize(16);
        labelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        labelText.SetText(L"Disabled");
        labelText.SetTextAlignment(AlignmentTypes::CenterMiddle);
        auto& labelRT = registry->GetComponent<RectTransformation>(wireframeModeStatusEntity);
        labelRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        labelRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        labelRT.SetSortingOrder(-1);
    }

    y += 20;

    {
        auto labelEntity = scene->GetUILayer()->CreateUIElement("Free Camera Mode label", debugWindowEntity);
        auto& labelText = registry->AddComponent<UIText>(labelEntity);
        labelText.SetTextSize(16);
        labelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        labelText.SetText(L"Free camera mode");
        auto& labelRT = registry->GetComponent<RectTransformation>(labelEntity);
        labelRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        labelRT.SetAnchorMax(glm::vec2(0.0f, 1.0f));
        labelRT.SetSize(glm::vec2(120, 20));
        labelRT.SetAnchoredPosition(glm::vec2(60 + 2, -y - 20 * 0.5f));
        labelRT.SetSortingOrder(-1);
    }
    {
        auto buttonEntity = scene->GetUILayer()->CreateUIElement("Free Camera Mode button", debugWindowEntity);
        auto& button = registry->AddComponent<UIButton>(buttonEntity);
        button.SetImage(buttonSprite);
        button.Callback = ChangeCameraMode;
        auto& buttonRT = registry->GetComponent<RectTransformation>(buttonEntity);
        buttonRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        buttonRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        buttonRT.SetSize(glm::vec2(80, 20));
        buttonRT.SetAnchoredPosition(glm::vec2(-40 - 2, -y - 20 * 0.5f));
        buttonRT.SetSortingOrder(-1);

        freeCameraModeStatusEntity = scene->GetUILayer()->CreateUIElement("Free Camera Mode status", buttonEntity);
        auto& labelText = registry->AddComponent<UIText>(freeCameraModeStatusEntity);
        labelText.SetTextSize(16);
        labelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        labelText.SetText(L"Disabled");
        labelText.SetTextAlignment(AlignmentTypes::CenterMiddle);
        auto& labelRT = registry->GetComponent<RectTransformation>(freeCameraModeStatusEntity);
        labelRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        labelRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        labelRT.SetSortingOrder(-1);
    }
}

int DebugInfoWindow::GetFPS()
{
    float smoothing = 0.9f;
    float currentFPS = Time::DeltaTime() == 0.0f ? 0.0f : 1.0f / Time::DeltaTime();
    lastFPS = (lastFPS * smoothing) + (currentFPS * (1.0f - smoothing));
    return (int)lastFPS;
}

void DebugInfoWindow::ChangeWireframeMode()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    Renderer::DrawWireframe = !Renderer::DrawWireframe;
    if (Renderer::DrawWireframe)
        registry->GetComponent<UIText>(wireframeModeStatusEntity).SetText(L"Enabled");
    else
        registry->GetComponent<UIText>(wireframeModeStatusEntity).SetText(L"Disabled");
}

void DebugInfoWindow::ChangeCameraMode()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    Debug::FreeCameraMode = !Debug::FreeCameraMode;
    if (Debug::FreeCameraMode)
        registry->GetComponent<UIText>(freeCameraModeStatusEntity).SetText(L"Enabled");
    else
        registry->GetComponent<UIText>(freeCameraModeStatusEntity).SetText(L"Disabled");
}
