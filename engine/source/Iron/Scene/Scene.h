#pragma once

#include <list>

#include "../EntityComponentSystem/EntitiesRegistry.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Framebuffer.h"
#include "../Resources/AsepriteData.h"
#include "../UI/UILayer.h"
#include "HierarchySystem.h"
#include "HierarchyParent.h"

class UILayer;
class TransformationSystem;

class Scene : public HierarchyParent
{
public:
    EntitiesRegistry* GetEntitiesRegistry();
    UILayer* GetUILayer();

    explicit Scene(bool initSystems = true);
    ~Scene() override;

    void CreateMainCamera();
    Camera& GetMainCamera();

    EntityID CreateEntity();
    EntityID CreateEntity(const char* name, EntityID parent);
    EntityID CreateEntity(AsepriteData& data);
    EntityID CreateEmptyEntity();
    void DestroyEntity(EntityID entity);

    void Update();
    void Draw(Framebuffer* framebuffer);

private:
    bool systemsInitialized = false;
    HierarchySystem* hierarchySystem;
    TransformationSystem* transformationSystem;

    EntitiesRegistry* entitiesRegistry;
    UILayer* uiLayer;
    std::list<EntityID> entitiesToDelete;
    EntityID _mainCameraEntity = NULL_ENTITY;

    void SortByHierarchy();
    void UpdateGlobalTransformation();
    void RefreshTransformation();
    void SortByDrawOrder();
    void CleanDestroyedEntities();
    void CleanAllEntities();
    void DestroyAndRemoveEntity(EntityID entity);
    void DestroyEntityInner(EntityID entity);
};
