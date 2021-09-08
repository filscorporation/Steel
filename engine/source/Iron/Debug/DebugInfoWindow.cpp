#include "DebugInfoWindow.h"
#include "../Core/Time.h"
#include "../Rendering/Renderer.h"
#include "../UI/UIElements/UIButton.h"
#include "Debug.h"

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
    registry->GetComponent<UIText>(fpsCounterEntity).SetText(std::to_string(GetFPS()));
    registry->GetComponent<UIText>(drawCallsCounterEntity).SetText(std::to_string(Renderer::DrawCallsStats));
    registry->GetComponent<UIText>(verticesCounterEntity).SetText(std::to_string(Renderer::VerticesStats));
}

void DebugInfoWindow::Create()
{
    auto resources = Application::Instance->GetResourcesManager();
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    auto windowSprite = resources->LoadSprite("debug_panel.png", true);
    auto buttonSprite = resources->LoadSprite("debug_button.png", true);

    {
        debugWindowEntity = scene->GetUILayer()->CreateUIImage(windowSprite, "Debug window", NULL_ENTITY);
        auto& windowRT = registry->GetComponent<RectTransformation>(debugWindowEntity);
        windowRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        windowRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        windowRT.SetSize(glm::vec2(200, 114));
        windowRT.SetAnchoredPosition(-windowRT.GetSize() * 0.5f - glm::vec2(2, 2));
    }

    float y = 6;
    float xOffset = 4;

    {
        auto fpsLabelEntity = scene->GetUILayer()->CreateUIText("FPS", "FPS label", debugWindowEntity);
        auto& fpsLabelText = registry->GetComponent<UIText>(fpsLabelEntity);
        fpsLabelText.SetTextSize(16);
        fpsLabelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        auto& fpsLabelRT = registry->GetComponent<RectTransformation>(fpsLabelEntity);
        fpsLabelRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        fpsLabelRT.SetAnchorMax(glm::vec2(0.0f, 1.0f));
        fpsLabelRT.SetSize(glm::vec2(100, 20));
        fpsLabelRT.SetAnchoredPosition(glm::vec2(50 + xOffset, -y - 20 * 0.5f));
    }
    {
        fpsCounterEntity = scene->GetUILayer()->CreateUIText("0", "FPS counter", debugWindowEntity);
        auto& fpsCounterText = registry->GetComponent<UIText>(fpsCounterEntity);
        fpsCounterText.SetTextSize(16);
        fpsCounterText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        auto& fpsCounterRT = registry->GetComponent<RectTransformation>(fpsCounterEntity);
        fpsCounterRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        fpsCounterRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        fpsCounterRT.SetSize(glm::vec2(100, 20));
        fpsCounterRT.SetAnchoredPosition(glm::vec2(-50 - 2, -y - 20 * 0.5f));
    }

    y += 20;

    {
        auto drawCallsLabelEntity = scene->GetUILayer()->CreateUIText("Draw calls", "Draw Calls label", debugWindowEntity);
        auto& drawCallsLabelText = registry->GetComponent<UIText>(drawCallsLabelEntity);
        drawCallsLabelText.SetTextSize(16);
        drawCallsLabelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        auto& drawCallsLabelRT = registry->GetComponent<RectTransformation>(drawCallsLabelEntity);
        drawCallsLabelRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        drawCallsLabelRT.SetAnchorMax(glm::vec2(0.0f, 1.0f));
        drawCallsLabelRT.SetSize(glm::vec2(100, 20));
        drawCallsLabelRT.SetAnchoredPosition(glm::vec2(50 + xOffset, -y - 20 * 0.5f));
    }
    {
        drawCallsCounterEntity = scene->GetUILayer()->CreateUIText("0", "Draw Calls counter", debugWindowEntity);
        auto& drawCallsCounterText = registry->GetComponent<UIText>(drawCallsCounterEntity);
        drawCallsCounterText.SetTextSize(16);
        drawCallsCounterText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        auto& drawCallsCounterRT = registry->GetComponent<RectTransformation>(drawCallsCounterEntity);
        drawCallsCounterRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        drawCallsCounterRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        drawCallsCounterRT.SetSize(glm::vec2(100, 20));
        drawCallsCounterRT.SetAnchoredPosition(glm::vec2(-50 - 2, -y - 20 * 0.5f));
    }

    y += 20;

    {
        auto verticesLabelEntity = scene->GetUILayer()->CreateUIText("Vertices count", "Vertices label", debugWindowEntity);
        auto& verticesLabelText = registry->GetComponent<UIText>(verticesLabelEntity);
        verticesLabelText.SetTextSize(16);
        verticesLabelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        auto& verticesLabelRT = registry->GetComponent<RectTransformation>(verticesLabelEntity);
        verticesLabelRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        verticesLabelRT.SetAnchorMax(glm::vec2(0.0f, 1.0f));
        verticesLabelRT.SetSize(glm::vec2(100, 20));
        verticesLabelRT.SetAnchoredPosition(glm::vec2(50 + xOffset, -y - 20 * 0.5f));
    }
    {
        verticesCounterEntity = scene->GetUILayer()->CreateUIText("0", "Vertices counter", debugWindowEntity);
        auto& verticesCounterText = registry->GetComponent<UIText>(verticesCounterEntity);
        verticesCounterText.SetTextSize(16);
        verticesCounterText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        auto& verticesCounterRT = registry->GetComponent<RectTransformation>(verticesCounterEntity);
        verticesCounterRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        verticesCounterRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        verticesCounterRT.SetSize(glm::vec2(100, 20));
        verticesCounterRT.SetAnchoredPosition(glm::vec2(-50 - 2, -y - 20 * 0.5f));
    }

    y += 20;

    {
        auto labelEntity = scene->GetUILayer()->CreateUIText("Wireframe mode", "Wireframe Mode label", debugWindowEntity);
        auto& labelText = registry->GetComponent<UIText>(labelEntity);
        labelText.SetTextSize(16);
        labelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        auto& labelRT = registry->GetComponent<RectTransformation>(labelEntity);
        labelRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        labelRT.SetAnchorMax(glm::vec2(0.0f, 1.0f));
        labelRT.SetSize(glm::vec2(120, 20));
        labelRT.SetAnchoredPosition(glm::vec2(60 + xOffset, -y - 20 * 0.5f));
    }
    {
        auto buttonEntity = scene->GetUILayer()->CreateUIButton(buttonSprite, "Wireframe Mode button", debugWindowEntity);
        auto& button = registry->GetComponent<UIButton>(buttonEntity);
        auto& buttonRT = registry->GetComponent<RectTransformation>(buttonEntity);
        buttonRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        buttonRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        buttonRT.SetSize(glm::vec2(80, 20));
        buttonRT.SetAnchoredPosition(glm::vec2(-40 - 2, -y - 20 * 0.5f));

        EntityID wireframeModeStatusEntity = scene->GetUILayer()->CreateUIText("Disabled", "Wireframe Mode status", buttonEntity);
        auto& labelText = registry->GetComponent<UIText>(wireframeModeStatusEntity);
        labelText.SetTextSize(16);
        labelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        labelText.SetTextAlignment(AlignmentTypes::CenterMiddle);
        auto& labelRT = registry->GetComponent<RectTransformation>(wireframeModeStatusEntity);
        labelRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        labelRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));

        button.Callback = [wireframeModeStatusEntity](EntityID entityID)
        {
            auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
            Renderer::DrawWireframe = !Renderer::DrawWireframe;
            if (Renderer::DrawWireframe)
                registry->GetComponent<UIText>(wireframeModeStatusEntity).SetText("Enabled");
            else
                registry->GetComponent<UIText>(wireframeModeStatusEntity).SetText("Disabled");
        };
    }

    y += 20;

    {
        auto labelEntity = scene->GetUILayer()->CreateUIText("Free camera mode", "Free Camera Mode label", debugWindowEntity);
        auto& labelText = registry->GetComponent<UIText>(labelEntity);
        labelText.SetTextSize(16);
        labelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        auto& labelRT = registry->GetComponent<RectTransformation>(labelEntity);
        labelRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        labelRT.SetAnchorMax(glm::vec2(0.0f, 1.0f));
        labelRT.SetSize(glm::vec2(120, 20));
        labelRT.SetAnchoredPosition(glm::vec2(60 + xOffset, -y - 20 * 0.5f));
    }
    {
        auto buttonEntity = scene->GetUILayer()->CreateUIButton(buttonSprite, "Free Camera Mode button", debugWindowEntity);
        auto& button = registry->GetComponent<UIButton>(buttonEntity);
        auto& buttonRT = registry->GetComponent<RectTransformation>(buttonEntity);
        buttonRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        buttonRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        buttonRT.SetSize(glm::vec2(80, 20));
        buttonRT.SetAnchoredPosition(glm::vec2(-40 - 2, -y - 20 * 0.5f));

        EntityID freeCameraModeStatusEntity = scene->GetUILayer()->CreateUIText("Disabled", "Free Camera Mode status", buttonEntity);
        auto& labelText = registry->GetComponent<UIText>(freeCameraModeStatusEntity);
        labelText.SetTextSize(16);
        labelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        labelText.SetTextAlignment(AlignmentTypes::CenterMiddle);
        auto& labelRT = registry->GetComponent<RectTransformation>(freeCameraModeStatusEntity);
        labelRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        labelRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));

        button.Callback = [freeCameraModeStatusEntity](EntityID entityID)
        {
            auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
            Debug::FreeCameraMode = !Debug::FreeCameraMode;
            if (Debug::FreeCameraMode)
                registry->GetComponent<UIText>(freeCameraModeStatusEntity).SetText("Enabled");
            else
                registry->GetComponent<UIText>(freeCameraModeStatusEntity).SetText("Disabled");
        };
    }
}

int DebugInfoWindow::GetFPS()
{
    float smoothing = 0.9f;
    float currentFPS = Time::DeltaTime() == 0.0f ? 0.0f : 1.0f / Time::DeltaTime();
    lastFPS = (lastFPS * smoothing) + (currentFPS * (1.0f - smoothing));
    return (int)lastFPS;
}
