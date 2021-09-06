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

    explicit Scene();
    ~Scene() override;
    void InitSystems();

    void CreateMainCamera();
    Camera& GetMainCamera();

    EntityID CreateEntity();
    EntityID CreateEntity(const char* name, EntityID parent);
    EntityID CreateEntity(AsepriteData& data);
    EntityID CreateEmptyEntity();
    void DestroyEntity(EntityID entity);

    void Update();
    void PrepareDraw();
    void Draw(Framebuffer* framebuffer);

protected:
    EntitiesRegistry* entitiesRegistry;
    UILayer* uiLayer;

    void RegisterSystems();

    // Methods to expand scene update and draw process
    virtual void BeforeUpdate() { };
    virtual void AfterUpdate() { };
    virtual void AfterPrepareDraw() { };
    virtual void BeforeDraw() { };
    virtual void AfterDraw() { };

private:
    bool systemsInitialized = false;
    HierarchySystem* hierarchySystem;
    TransformationSystem* transformationSystem;
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
