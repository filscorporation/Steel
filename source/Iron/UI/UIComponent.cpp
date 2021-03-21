#include "../Scene/SceneHelper.h"
#include "UIComponent.h"
#include "UIRenderer.h"
#include "UIEventHandler.h"

UIComponent::UIComponent(EntityID ownerEntityID) : Component(ownerEntityID)
{
    if (!HasComponentS<RectTransformation>(Owner))
    {
        Log::LogError("Adding UIComponents to objects without RectTransformation will lead to undefined behaviour. "
                      "Create UI objects using UILayout, not Scene");
    }

    if (HasComponentS<UIRenderer>(ownerEntityID))
    {
        Log::LogError("Entity already has UIRenderer component attached, this may lead to conflicts. Keep one UIComponent per object");
    }
    else
    {
        AddComponentS<UIRenderer>(ownerEntityID);
    }

    if (HasComponentS<UIEventHandler>(ownerEntityID))
    {
        Log::LogError("Entity already has UIEventHandler component attached, this may lead to conflicts. Keep one UIEventHandler per object");
    }
    else
    {
        AddComponentS<UIEventHandler>(ownerEntityID);
    }
}

RectTransformation& UIComponent::GetRectTransform()
{
    return GetComponentS<RectTransformation>(Owner);
}
