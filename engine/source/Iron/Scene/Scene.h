#pragma once

#include <list>

#include "../EntityComponentSystem/EntitiesRegistry.h"
#include "../Rendering/Camera.h"
#include "../Resources/AsepriteData.h"
#include "../UI/UILayer.h"
#include "HierarchySystem.h"
#include "TransformationSystem.h"

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
    EntityID CreateEntity(AsepriteData& data);
    EntityID CreateEmptyEntity();
    void DestroyEntity(EntityID entity);

    void SortByHierarchy();
    void UpdateGlobalTransformation();
    void RefreshTransformation();
    void SortByDrawOrder();
    void Draw();
    void CleanDestroyedEntities();
    void CleanAllEntities();

    Camera& GetMainCamera();

    static int EntitiesWasCreated;

private:
    HierarchySystem* hierarchySystem;
    TransformationSystem* transformationSystem;

    EntitiesRegistry* entitiesRegistry;
    UILayer* uiLayer;
    std::list<EntityID> entitiesToDelete;
    EntityID _mainCameraEntity;
    void DestroyAndRemoveEntity(EntityID entity);
    void DestroyEntityInner(EntityID entity);
};
