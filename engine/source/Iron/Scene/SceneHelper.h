#pragma once

#include "../Core/Application.h"

// TODO: temp solution

// Shortcuts
template<class T>
T& AddComponentS(EntityID entityID)
{
    return Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->AddComponent<T>(entityID);
}

template<class T>
T& GetComponentS(EntityID entityID)
{
    return Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->GetComponent<T>(entityID);
}

template<class T>
bool HasComponentS(EntityID entityID)
{
    return Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->HasComponent<T>(entityID);
}

template<class T>
bool RemoveComponentS(EntityID entityID)
{
    return Application::Instance->GetCurrentScene()->GetEntitiesRegistry()->RemoveComponent<T>(entityID);
}
