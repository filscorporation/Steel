#include "ProjectViewElement.h"
#include "../EditorCore/EditorBuilder.h"

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
