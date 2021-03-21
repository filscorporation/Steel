#include "UIButton.h"
#include "UIEventHandler.h"
#include "../Scene/SceneHelper.h"

UIButton::UIButton(EntityID ownerEntityID) : UIImage(ownerEntityID)
{
    auto& eh = GetComponentS<UIEventHandler>(ownerEntityID);

    eh.EventCallback = HandleEvent;
    eh.EventsMask = UIEventTypes::MouseEnter | UIEventTypes::MouseExit
                  | UIEventTypes::MouseJustPressed | UIEventTypes::MouseJustReleased;
}

void UIButton::HandleEvent(EntityID handler, UIEventTypes::UIEventType eventType, UIEvent& uiEvent)
{
    GetComponentS<UIButton>(handler).HandleEventInner(eventType, uiEvent);
}

void UIButton::HandleEventInner(UIEventTypes::UIEventType eventType, UIEvent &uiEvent)
{
    if (eventType & UIEventTypes::MouseEnter)
    {
        auto& uii = GetComponentS<UIRenderer>(Owner);
        uii._color = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
    }
    if (eventType & UIEventTypes::MouseExit)
    {
        auto& uii = GetComponentS<UIRenderer>(Owner);
        uii._color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    if (eventType & UIEventTypes::MouseJustPressed)
    {
        Log::LogInfo("Clicked button with ID " + std::to_string(Owner));
    }
}
