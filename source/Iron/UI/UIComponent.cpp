#include "UIComponent.h"
#include "../Scene/SceneHelper.h"

UIComponent::UIComponent(EntityID ownerEntityID) : Component(ownerEntityID)
{
    if (!HasComponentS<RectTransformation>(Owner))
    {
        Log::LogError("Adding UIComponents to objects without RectTransformation will lead to undefined behaviour. "
                      "Create UI objects using UILayout, not Scene");
    }
}

RectTransformation& UIComponent::GetRectTransform()
{
    return GetComponentS<RectTransformation>(Owner);
}
