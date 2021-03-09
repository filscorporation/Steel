#include "UIComponent.h"
#include "../Scene/SceneHelper.h"

UIComponent::UIComponent(EntityID ownerEntityID) : Component(ownerEntityID)
{
    // TODO: split ui and normal objects creation
    RemoveComponentS<Transformation>(Owner);
    AddComponentS<RectTransformation>(Owner);
}

RectTransformation& UIComponent::GetRectTransform()
{
    return GetComponentS<RectTransformation>(Owner);
}
