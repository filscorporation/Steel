#include "ControlPanel.h"
#include "../EditorCore/EditorApplication.h"
#include "../EditorCore/EditorBuilder.h"
#include "../EditorCore/EditorSceneManager.h"

void ControlPanel::Init()
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
    auto resources = Application::Context()->Resources;
    auto entitiesRegistry = Application::Context()->Scenes->GetActiveScene()->GetEntitiesRegistry();

    auto& tabImage = entitiesRegistry->AddComponent<UIImage>(Owner);
    tabImage.SetImage(layer->UIResources.DefaultPixelSprite);
    tabImage.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    playButtonSprite = resources->LoadSprite("play_button.png");
    stopButtonSprite = resources->LoadSprite("stop_button.png");
    pauseButtonSprite = resources->LoadSprite("pause_button.png");
    stepButtonSprite = resources->LoadSprite("step_button.png");

    UpdateState();
}

void ControlPanel::Update(EntitiesRegistry* entitiesRegistry)
{
    auto editor = (EditorApplication*)Application::Instance;
    if (editor->GetState() != lastState)
        UpdateState();
}

void ControlPanel::UpdateState()
{
    auto editor = (EditorApplication*)Application::Instance;
    lastState = editor->GetState();
    if (lastState == EditorStates::Playing || lastState == EditorStates::Paused)
        CreateStopButton();
    else
        CreatePlayButton();
    CreatePauseButton();
    CreateStepButton();
}

void ControlPanel::CreatePlayButton()
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
    auto entitiesRegistry = Application::Context()->Scenes->GetActiveScene()->GetEntitiesRegistry();

    if (playButtonEntity != NULL_ENTITY)
        entitiesRegistry->DeleteEntity(playButtonEntity);

    auto sprite = playButtonSprite;
    playButtonEntity = layer->CreateUIButton(sprite, "Play button", Owner);
    auto& buttonRT = entitiesRegistry->GetComponent<RectTransformation>(playButtonEntity);
    buttonRT.SetSize(glm::vec2(sprite->SpriteTexture->GetWidth(), sprite->SpriteTexture->GetHeight()));
    buttonRT.SetAnchorMin(glm::vec2(0.5f, 0.5f));
    buttonRT.SetAnchorMax(glm::vec2(0.5f, 0.5f));
    buttonRT.SetAnchoredPosition(glm::vec2(-STYLE_OFFSET - STYLE_BUTTON_H * 2.25f, 0.0f));

    auto& button = entitiesRegistry->GetComponent<UIButton>(playButtonEntity);
    EntityID controlPanelEntity = Owner;
    button.Callback = [controlPanelEntity](EntityID entityID)
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        registry->GetComponent<ControlPanel>(controlPanelEntity).PlayButtonPressed();
    };
}

void ControlPanel::CreateStopButton()
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
    auto entitiesRegistry = Application::Context()->Scenes->GetActiveScene()->GetEntitiesRegistry();

    if (stopButtonEntity != NULL_ENTITY)
        entitiesRegistry->DeleteEntity(stopButtonEntity);

    auto sprite = stopButtonSprite;
    stopButtonEntity = layer->CreateUIButton(sprite, "Stop button", Owner);
    auto& buttonRT = entitiesRegistry->GetComponent<RectTransformation>(stopButtonEntity);
    buttonRT.SetSize(glm::vec2(sprite->SpriteTexture->GetWidth(), sprite->SpriteTexture->GetHeight()));
    buttonRT.SetAnchorMin(glm::vec2(0.5f, 0.5f));
    buttonRT.SetAnchorMax(glm::vec2(0.5f, 0.5f));
    buttonRT.SetAnchoredPosition(glm::vec2(-STYLE_OFFSET - STYLE_BUTTON_H * 2.25f, 0.0f));

    auto& button = entitiesRegistry->GetComponent<UIButton>(stopButtonEntity);
    EntityID controlPanelEntity = Owner;
    button.Callback = [controlPanelEntity](EntityID entityID)
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        registry->GetComponent<ControlPanel>(controlPanelEntity).StopButtonPressed();
    };
}

void ControlPanel::CreatePauseButton()
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
    auto entitiesRegistry = Application::Context()->Scenes->GetActiveScene()->GetEntitiesRegistry();

    if (pauseButtonEntity != NULL_ENTITY)
        entitiesRegistry->DeleteEntity(pauseButtonEntity);

    auto sprite = pauseButtonSprite;
    pauseButtonEntity = layer->CreateUIButton(sprite, "Pause button", Owner);
    auto& buttonRT = entitiesRegistry->GetComponent<RectTransformation>(pauseButtonEntity);
    buttonRT.SetSize(glm::vec2(sprite->SpriteTexture->GetWidth(), sprite->SpriteTexture->GetHeight()));
    buttonRT.SetAnchorMin(glm::vec2(0.5f, 0.5f));
    buttonRT.SetAnchorMax(glm::vec2(0.5f, 0.5f));
    buttonRT.SetAnchoredPosition(glm::vec2(0.0f, 0.0f));

    auto& button = entitiesRegistry->GetComponent<UIButton>(pauseButtonEntity);
    EntityID controlPanelEntity = Owner;
    button.Callback = [controlPanelEntity](EntityID entityID)
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        registry->GetComponent<ControlPanel>(controlPanelEntity).PauseButtonPressed();
    };
}

void ControlPanel::CreateStepButton()
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
    auto entitiesRegistry = Application::Context()->Scenes->GetActiveScene()->GetEntitiesRegistry();

    if (stepButtonEntity != NULL_ENTITY)
        entitiesRegistry->DeleteEntity(stepButtonEntity);

    auto sprite = stepButtonSprite;
    stepButtonEntity = layer->CreateUIButton(sprite, "Step button", Owner);
    auto& buttonRT = entitiesRegistry->GetComponent<RectTransformation>(stepButtonEntity);
    buttonRT.SetSize(glm::vec2(sprite->SpriteTexture->GetWidth(), sprite->SpriteTexture->GetHeight()));
    buttonRT.SetAnchorMin(glm::vec2(0.5f, 0.5f));
    buttonRT.SetAnchorMax(glm::vec2(0.5f, 0.5f));
    buttonRT.SetAnchoredPosition(glm::vec2(STYLE_OFFSET + STYLE_BUTTON_H * 2.25f, 0.0f));

    auto& button = entitiesRegistry->GetComponent<UIButton>(stepButtonEntity);
    EntityID controlPanelEntity = Owner;
    button.Callback = [controlPanelEntity](EntityID entityID)
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        registry->GetComponent<ControlPanel>(controlPanelEntity).StepButtonPressed();
    };
    button.SetInteractable(lastState == EditorStates::Paused || lastState == EditorStates::Step);
}

void ControlPanel::PlayButtonPressed()
{
    auto editor = (EditorApplication*)Application::Instance;

    editor->SetState(EditorStates::Playing);
    UpdateState();
}

void ControlPanel::StopButtonPressed()
{
    auto editor = (EditorApplication*)Application::Instance;

    editor->SetState(EditorStates::Stopped);
    UpdateState();
}

void ControlPanel::PauseButtonPressed()
{
    auto editor = (EditorApplication*)Application::Instance;

    editor->SetState(editor->GetState() == EditorStates::Paused ? EditorStates::Playing : EditorStates::Paused);
    UpdateState();
}

void ControlPanel::StepButtonPressed()
{
    auto editor = (EditorApplication*)Application::Instance;

    editor->SetState(EditorStates::Step);
    UpdateState();
}