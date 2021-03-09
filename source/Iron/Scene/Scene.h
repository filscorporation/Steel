#pragma once

#include <list>

#include "../EntityComponentSystem/EntitiesRegistry.h"
#include "../Rendering/Camera.h"
#include "../Core/Application.h"

class Scene
{
public:
    EntitiesRegistry* GetEntitiesRegistry();

    Scene();
    ~Scene();
    void CreateMainCamera();
    EntityID CreateEntity();
    void DestroyEntity(EntityID entity);
    bool IsEntityDestroyed(EntityID entity);
    void CleanDestroyedEntities();
    void CleanAllEntities();

    Camera& GetMainCamera();

    static int EntitiesWasCreated;

private:
    EntitiesRegistry* entitiesRegistry;
    std::list<EntityID> entitiesToDelete;
    EntityID _mainCameraEntity;
    void DestroyAndRemoveEntity(EntityID entity);
    void DestroyEntityInner(EntityID entity);
};
