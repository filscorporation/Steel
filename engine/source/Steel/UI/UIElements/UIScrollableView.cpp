#include "UIScrollableView.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/UI/UIEventHandler.h"

#define SCROLL_SENSITIVITY 25.0f

void UIScrollableView::RegisterType()
{
    REGISTER_COMPONENT(UIScrollableView);
    REGISTER_ID_ATTRIBUTE(UIScrollableView, "content", GetContent, SetContent, AttributeFlags::Public, "Content");
}

bool UIScrollableView::Validate(EntitiesRegistry* entitiesRegistry)
{
    return Component::Validate(entitiesRegistry) && CheckRectTransformation(entitiesRegistry);
}

void UIScrollableView::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    auto& eventHandler = entitiesRegistry->AddComponent<UIEventHandler>(Owner);
    eventHandler.EventCallback = UIScrollableView::HandleEvent;
    eventHandler.EventsMask = UIEventTypes::ScrollDelta | UIEventTypes::MouseOver;

    auto layer = Application::Instance->GetCurrentScene()->GetUILayer();

    if (content == NULL_ENTITY)
    {
        content = layer->CreateUIElement("Content", Owner);
        auto& contentRT = entitiesRegistry->GetComponent<RectTransformation>(content);
        contentRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        contentRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        contentRT.SetOffsetMin(glm::vec2(0.0f, 0.0f));
        contentRT.SetOffsetMax(glm::vec2(0.0f, 0.0f));
        contentRT.SetPivot(glm::vec2(0.5f, 1.0f));
    }
}

void UIScrollableView::HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    GetComponentS<UIScrollableView>(handler).HandleEventInner(eventType, uiEvent);
}

void UIScrollableView::HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    if (eventType & UIEventTypes::ScrollDelta)
    {
        auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        if (!entitiesRegistry->EntityExists(content))
            return;

        auto& rectTransformation = entitiesRegistry->GetComponent<RectTransformation>(Owner);
        bool contains = rectTransformation.Contains(uiEvent.MousePosition);
        if (!contains)
            return;

        auto& contentRT = entitiesRegistry->GetComponent<RectTransformation>(content);
        float size = rectTransformation.GetRealSizeCached().y;
        const glm::vec2& contentPosition = contentRT.GetAnchoredPosition();
        float contentSize = contentRT.GetRealSizeCached().y;

        float newY = Math::Clamp(contentPosition.y - uiEvent.ScrollDelta.y * SCROLL_SENSITIVITY, 0.0f, std::max(0.0f, contentSize - size));
        contentRT.SetAnchoredPosition(glm::vec2(contentPosition.x, newY));
    }
}

EntityID UIScrollableView::GetContent() const
{
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    return entitiesRegistry->EntityExists(content) ? content : NULL_ENTITY;
}

void UIScrollableView::SetContent(EntityID contentEntityID)
{
    content = contentEntityID;
}
