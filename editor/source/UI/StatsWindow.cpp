#include "StatsWindow.h"
#include "../EditorCore/EditorApplication.h"

#include <Steel.h>

void StatsWindow::RegisterType()
{
    REGISTER_COMPONENT(StatsWindow);
}

void StatsWindow::UpdateInfo()
{
    auto editor = (EditorApplication*)Application::Instance;
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    UpdateFPS();
    fpsTextUpdateTimer += Time::UnscaledDeltaTime();
    if (fpsTextUpdateTimer > 0.2f)
    {
        registry->GetComponent<UIText>(fpsCounterEntity).SetText(std::to_string(GetFPS()));
        fpsTextUpdateTimer = 0.0f;
    }

    registry->GetComponent<UIText>(drawCallsCounterEntity).SetText(std::to_string(editor->GetAppContext()->Stats.DrawCalls));
    registry->GetComponent<UIText>(verticesCounterEntity).SetText(std::to_string(editor->GetAppContext()->Stats.VerticesCount));
}

void StatsWindow::Init()
{
    for (int i = 0; i < FPS_FRAMES_COUNT; ++i)
        framesList[i] = 0;

    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    auto frameSprite = scene->GetUILayer()->UIResources.DefaultFrameSprite;

    registry->AddComponent<UIImage>(Owner).SetImage(frameSprite);

    float y = 6;
    float xOffset = 4;

    {
        auto fpsLabelEntity = scene->GetUILayer()->CreateUIText("FPS", "FPS label", Owner);
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
        fpsCounterEntity = scene->GetUILayer()->CreateUIText("0", "FPS counter", Owner);
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
        auto drawCallsLabelEntity = scene->GetUILayer()->CreateUIText("Draw calls", "Draw Calls label", Owner);
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
        drawCallsCounterEntity = scene->GetUILayer()->CreateUIText("0", "Draw Calls counter", Owner);
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
        auto verticesLabelEntity = scene->GetUILayer()->CreateUIText("Vertices count", "Vertices label", Owner);
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
        verticesCounterEntity = scene->GetUILayer()->CreateUIText("0", "Vertices counter", Owner);
        auto& verticesCounterText = registry->GetComponent<UIText>(verticesCounterEntity);
        verticesCounterText.SetTextSize(16);
        verticesCounterText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        auto& verticesCounterRT = registry->GetComponent<RectTransformation>(verticesCounterEntity);
        verticesCounterRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        verticesCounterRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        verticesCounterRT.SetSize(glm::vec2(100, 20));
        verticesCounterRT.SetAnchoredPosition(glm::vec2(-50 - 2, -y - 20 * 0.5f));
    }
}

void StatsWindow::UpdateFPS()
{
    // here we fill buffer with delta time of previous frames to calculate average value
    // if frame is too long - we write delta multiple times to update average faster
    frameTimer += Time::DeltaTime();
    do
    {
        framesList[frameIndex] = Time::DeltaTime();
        frameIndex = (frameIndex + 1) % FPS_FRAMES_COUNT;
        frameTimer -= FPS_MAX_DELTA;
    }
    while (frameTimer >= FPS_MAX_DELTA);

    if (frameTimer < 0)
        frameTimer = 0;
}

int StatsWindow::GetFPS()
{
    float fps = 0;
    for (int i = 0; i < FPS_FRAMES_COUNT; ++i)
        fps += framesList[i];
    fps /= FPS_FRAMES_COUNT;
    fps = 1.0f / fps;

    return std::ceil(fps);
}
