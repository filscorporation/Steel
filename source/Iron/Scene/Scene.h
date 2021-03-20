#pragma once

#include <list>

#include "../EntityComponentSystem/EntitiesRegistry.h"
#include "../Rendering/Camera.h"
#include "../Core/Application.h"
#include "../UI/UILayer.h"

class UILayer;

class Scene
{
public:
    EntitiesRegistry* GetEntitiesRegistry();
    UILayer* GetUILayer();

    Scene();
    ~Scene();
    void CreateMainCamera();
    EntityID CreateEntity();
    EntityID CreateEntity(const char* name, EntityID parent);
    EntityID CreateEmptyEntity();
    void DestroyEntity(EntityID entity);

    void SortByHierarchy();
    void UpdateGlobalTransformation();
    void CleanDestroyedEntities();
    void CleanAllEntities();

    Camera& GetMainCamera();

    static int EntitiesWasCreated;

private:
    EntitiesRegistry* entitiesRegistry;
    UILayer* uiLayer;
    std::list<EntityID> entitiesToDelete;
    EntityID _mainCameraEntity;
    void DestroyAndRemoveEntity(EntityID entity);
    void DestroyEntityInner(EntityID entity);
};
