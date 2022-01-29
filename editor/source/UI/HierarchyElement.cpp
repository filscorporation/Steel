#include "HierarchyElement.h"
#include "../EditorCore/EditorBuilder.h"

#include <Steel/Scene/NameComponent.h>

void HierarchyElement::RegisterType()
{
    REGISTER_COMPONENT(HierarchyElement);
}

void HierarchyElement::Init(EntitiesRegistry* entitiesRegistry, EntitiesRegistry* sceneRegistry, UILayer* layer,
                            const HierarchyView& hierarchyView, EntityID nodeEntity, const HierarchyViewNode& node)
{
    EntityID hierarchyViewEntity = hierarchyView.Owner;
    EntityID elementEntity = Owner;

    SetPosition(entitiesRegistry, node.Order);

    buttonEntityID = layer->CreateUIButton(layer->UIResources.DefaultStylePixelSprite, "Node", Owner);
    auto& buttonRT = entitiesRegistry->GetComponent<RectTransformation>(buttonEntityID);
    buttonRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    buttonRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));

    auto& button = entitiesRegistry->GetComponent<UIButton>(buttonEntityID);
    button.Callback = [hierarchyViewEntity, elementEntity](EntityID entityID)
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        registry->GetComponent<HierarchyView>(hierarchyViewEntity).ElementClicked(elementEntity);
    };

    auto& nodeName = sceneRegistry->GetComponent<NameComponent>(nodeEntity);
    EntityID textEntity = layer->CreateUIText(nodeName.GetName(), "Node", buttonEntityID);
    auto& text = entitiesRegistry->GetComponent<UIText>(textEntity);
    if (node.Flags & NodeFlags::Active)
        text.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    else
        text.SetColor(STYLE_MID_GREY);
    auto& nodeHN = sceneRegistry->GetComponent<HierarchyNode>(nodeEntity);
    auto& textRT = entitiesRegistry->GetComponent<RectTransformation>(textEntity);
    textRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    textRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    textRT.SetOffsetMin(glm::vec2(8.0f + STYLE_BUTTON_H + STYLE_OFFSET + STYLE_BUTTON_H * nodeHN.GetHierarchyDepth(), 2.0f));
    textRT.SetOffsetMax(glm::vec2(8.0f, 2.0f));

    if (node.HasChildren)
    {
        EntityID arrowEntityID = layer->CreateUIButton(layer->UIResources.DefaultArrowSprite, "Arrow", buttonEntityID);
        auto& arrowRT = entitiesRegistry->GetComponent<RectTransformation>(arrowEntityID);
        arrowRT.SetAnchorMin(glm::vec2(0.0f, 0.5f));
        arrowRT.SetAnchorMax(glm::vec2(0.0f, 0.5f));
        if (node.Flags & NodeFlags::Expanded)
            arrowRT.SetRotation(glm::vec3(0.0f, 0.0f, -Math::Pi * 0.5f));
        auto arrowTexture = layer->UIResources.DefaultArrowSprite->SpriteTexture;
        arrowRT.SetSize(glm::vec2(arrowTexture->GetWidth(), arrowTexture->GetHeight()));
        arrowRT.SetAnchoredPosition(glm::vec2(8.0f + STYLE_BUTTON_H * 0.5f + STYLE_BUTTON_H * nodeHN.GetHierarchyDepth(), 0.0f));

        auto& arrowButton = entitiesRegistry->GetComponent<UIButton>(arrowEntityID);
        arrowButton.Callback = [hierarchyViewEntity, elementEntity](EntityID entityID)
        {
            auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
            registry->GetComponent<HierarchyView>(hierarchyViewEntity).ElementExpanded(elementEntity);
        };
    }

    if (node.Flags & NodeFlags::Selected)
        SetSelectedNodeStyle(entitiesRegistry);
    else
        SetDefaultNodeStyle(entitiesRegistry);
}

void HierarchyElement::UpdatePosition(EntitiesRegistry* entitiesRegistry, EntitiesRegistry* sceneRegistry, EntityID nodeEntity, const HierarchyViewNode& node)
{
    auto& nodeHN = sceneRegistry->GetComponent<HierarchyNode>(nodeEntity);
    SetPosition(entitiesRegistry, node.Order);
}

void HierarchyElement::SetDefaultNodeStyle(EntitiesRegistry* entitiesRegistry) const
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

void HierarchyElement::SetSelectedNodeStyle(EntitiesRegistry* entitiesRegistry) const
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

void HierarchyElement::SetPosition(EntitiesRegistry* entitiesRegistry, int order)
{
    auto& rectTransformation = entitiesRegistry->GetComponent<RectTransformation>(Owner);
    rectTransformation.SetAnchorMin(glm::vec2(0.0f, 1.0f));
    rectTransformation.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    rectTransformation.SetSize(glm::vec2(0.0f, STYLE_BUTTON_H));
    rectTransformation.SetPivot(glm::vec2(0.0f, 1.0f));
    rectTransformation.SetAnchoredPosition(glm::vec2(0.0f, -STYLE_BUTTON_H * order));
}
