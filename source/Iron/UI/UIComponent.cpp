#include "UIComponent.h"
#include "../Scene/Entity.h"

UIComponent::UIComponent(Entity *parentEntity) : Component(parentEntity)
{
    // TODO: split ui and normal objects creation
    ParentEntity->RemoveComponent<Transformation>();
    ParentEntity->Transform = ParentEntity->AddComponent<RectTransformation>();
}

RectTransformation *UIComponent::GetRectTransform()
{
    return (RectTransformation*)ParentEntity->Transform;
}
