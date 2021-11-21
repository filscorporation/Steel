#pragma once

#include <Steel/UI/UIComponent.h>
#include <Steel/UI/UILayer.h>
#include <Steel/UI/UIElements/UIButton.h>
#include <Steel/EntityComponentSystem/EntitiesRegistry.h>
#include "../EditorCore/EditorApplication.h"

class ControlPanel : public UIComponent
{
    COMPONENT(ControlPanel)

public:
    explicit ControlPanel(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void Init();
    void Update(EntitiesRegistry* entitiesRegistry);

private:
    EditorStates::EditorState lastState = EditorStates::Stopped;
    EntityID playButtonEntity = NULL_ENTITY;
    EntityID stopButtonEntity = NULL_ENTITY;
    EntityID pauseButtonEntity = NULL_ENTITY;
    EntityID stepButtonEntity = NULL_ENTITY;

    // TODO: remove after resources manager rework
    Sprite* playButtonSprite = nullptr;
    Sprite* stopButtonSprite = nullptr;
    Sprite* pauseButtonSprite = nullptr;
    Sprite* stepButtonSprite = nullptr;

    void SaveScene();
    void LoadScene();

    void UpdateState();
    void CreatePlayButton();
    void CreateStopButton();
    void CreatePauseButton();
    void CreateStepButton();

    void PlayButtonPressed();
    void StopButtonPressed();
    void PauseButtonPressed();
    void StepButtonPressed();
};
