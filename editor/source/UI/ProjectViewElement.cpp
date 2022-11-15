#include "ProjectViewElement.h"
#include "../EditorCore/EditorBuilder.h"

#include <Steel/Serialization/AttributesRegistration.h>

void ProjectViewElement::RegisterType()
{
    REGISTER_COMPONENT(ProjectViewElement);
}

void ProjectViewElement::Init(EntitiesRegistry* entitiesRegistry, UILayer* layer, const ProjectView& projectView, const ProjectViewNode& node)
{
    EntityID projectViewEntity = projectView.Owner;
    EntityID elementEntity = Owner;

    SetPosition(entitiesRegistry, node.Order);

    buttonEntityID = layer->CreateUIButton(layer->UIResources.DefaultStylePixelSprite, "Node", Owner);
    auto& buttonRT = entitiesRegistry->GetComponent<RectTransformation>(buttonEntityID);
    buttonRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    buttonRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));

    auto& button = entitiesRegistry->GetComponent<UIButton>(buttonEntityID);
    button.Callback = [projectViewEntity, elementEntity](EntityID entityID)
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        registry->GetComponent<ProjectView>(projectViewEntity).ElementClicked(elementEntity);
    };

    std::string label = (node.IsDirectory ? "/" : "") + node.FileName;
    EntityID textEntity = layer->CreateUIText(label, "Node", buttonEntityID);
    auto& text = entitiesRegistry->GetComponent<UIText>(textEntity);
    text.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    auto& textRT = entitiesRegistry->GetComponent<RectTransformation>(textEntity);
    textRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    textRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    textRT.SetOffsetMin(glm::vec2(8.0f + STYLE_BUTTON_H + STYLE_OFFSET, 2.0f));
    textRT.SetOffsetMax(glm::vec2(8.0f, 2.0f));
}

void ProjectViewElement::SetPosition(EntitiesRegistry* entitiesRegistry, int order)
{
    auto& rectTransformation = entitiesRegistry->GetComponent<RectTransformation>(Owner);
    rectTransformation.SetAnchorMin(glm::vec2(0.0f, 1.0f));
    rectTransformation.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    rectTransformation.SetSize(glm::vec2(0.0f, STYLE_BUTTON_H));
    rectTransformation.SetPivot(glm::vec2(0.0f, 1.0f));
    rectTransformation.SetAnchoredPosition(glm::vec2(0.0f, -STYLE_BUTTON_H * order));
}

void ProjectViewElement::SetDefaultNodeStyle(EntitiesRegistry* entitiesRegistry) const
{
    auto& uiButton = entitiesRegistry->GetComponent<UIButton>(buttonEntityID);

    TransitionsInfo transitionsInfo {};
    transitionsInfo.TransitionType = TransitionTypes::ColorShift;
    transitionsInfo.TransitionDuration = 0.1f;
    transitionsInfo.Normal.FromColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    transitionsInfo.Hovered.FromColor(glm::vec4(0.85f, 0.85f, 0.85f, 1.0f));
    transitionsInfo.Selected.FromColor(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
    transitionsInfo.Clicked.FromColor(glm::vec4(0.55f, 0.55f, 0.55f, 1.0f));
    transitionsInfo.Disabled.FromColor(glm::vec4(0.35f, 0.35f, 0.35f, 1.0f));

    uiButton.SetTransitionsInfo(transitionsInfo);
}

void ProjectViewElement::SetSelectedNodeStyle(EntitiesRegistry* entitiesRegistry) const
{
    auto& uiButton = entitiesRegistry->GetComponent<UIButton>(buttonEntityID);

    const float WHITE_COMPONENT = 0.8f;
    const float BLUE_COMPONENT = 1.0f;

    TransitionsInfo transitionsInfo {};
    transitionsInfo.TransitionType = TransitionTypes::ColorShift;
    transitionsInfo.TransitionDuration = 0.1f;
    transitionsInfo.Normal.FromColor(glm::vec4(WHITE_COMPONENT * 1.0f, WHITE_COMPONENT * 1.0f, BLUE_COMPONENT * 1.0f, 1.0f));
    transitionsInfo.Hovered.FromColor(glm::vec4(WHITE_COMPONENT * 0.85f, WHITE_COMPONENT * 0.85f, BLUE_COMPONENT * 0.85f, 1.0f));
    transitionsInfo.Selected.FromColor(glm::vec4(WHITE_COMPONENT * 0.75f, WHITE_COMPONENT * 0.75f, BLUE_COMPONENT * 0.75f, 1.0f));
    transitionsInfo.Clicked.FromColor(glm::vec4(WHITE_COMPONENT * 0.55f, WHITE_COMPONENT * 0.55f, BLUE_COMPONENT * 0.55f, 1.0f));
    transitionsInfo.Disabled.FromColor(glm::vec4(WHITE_COMPONENT * 0.35f, WHITE_COMPONENT * 0.35f, BLUE_COMPONENT * 0.35f, 1.0f));

    uiButton.SetTransitionsInfo(transitionsInfo);
}
