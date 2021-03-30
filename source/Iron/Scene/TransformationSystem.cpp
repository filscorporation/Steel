#include "TransformationSystem.h"

void TransformationSystem::OnComponentAdded(EntityID entityID, Transformation& component)
{

}

void TransformationSystem::OnComponentRemoved(EntityID entityID, Transformation& component)
{

}

void TransformationSystem::OnEntityEnabled(EntityID entityID, Transformation& component)
{
    component.SetTransformationChanged(true);
}

void TransformationSystem::OnEntityDisabled(EntityID entityID, Transformation& component)
{

}

void TransformationSystem::OnComponentAdded(EntityID entityID, RectTransformation& component)
{

}

void TransformationSystem::OnComponentRemoved(EntityID entityID, RectTransformation& component)
{

}

void TransformationSystem::OnEntityEnabled(EntityID entityID, RectTransformation& component)
{
    component.SetTransformationChanged(true);
}

void TransformationSystem::OnEntityDisabled(EntityID entityID, RectTransformation& component)
{

}
