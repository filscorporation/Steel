#include "UIComponent.h"
#include "../Scene/Entity.h"

RectTransformation *UIComponent::GetRectTransform()
{
    return (RectTransformation*)ParentEntity->Transform;
}
