#pragma once

#include <list>

#include "../EntityComponentSystem/EntitiesRegistry.h"
#include "../Rendering/Camera.h"
#include "../Rendering/Framebuffer.h"
#include "../Resources/AsepriteData.h"
#include "../UI/UILayer.h"
#include "HierarchyParent.h"

class UILayer;

class Scene : public HierarchyParent
{
public:
    EntitiesRegistry* GetEntitiesRegistry();
    UILayer* GetUILayer();

    explicit Scene();
    Scene(const Scene& scene);
    ~Scene() override;
    void Init();

    void CreateMainCamera();
    EntityID GetMainCamera();

    EntityID CreateEntity();
    EntityID CreateEntity(const char* name, EntityID parent);
    EntityID CreateEntity(AsepriteData& data);
    EntityID CreateEmptyEntity();
    void DestroyEntity(EntityID entity);
    void CleanDestroyedEntities();

    void Refresh();
    void Update();
    void PrepareDraw();

protected:
    EntitiesRegistry* entitiesRegistry;
    UILayer* uiLayer;

    // Methods to expand scene update and draw process
    virtual void BeforeUpdate() { };
    virtual void AfterUpdate() { };
    virtual void BeforeRebuildUI() { };
    virtual void AfterPrepareDraw() { };
    virtual void BeforeDraw() { };
    virtual void AfterDraw() { };

private:
    bool systemsInitialized = false;
    std::list<EntityID> entitiesToDelete;
    EntityID _mainCameraEntity = NULL_ENTITY;

    void SortByHierarchy();
    void UpdateGlobalTransformation();
    void RefreshTransformation();
    void SortByDrawOrder();
    void CleanAllEntities();
    void DestroyAndRemoveEntity(EntityID entity);
    void DestroyEntityInner(EntityID entity);

    friend class SceneRenderer;
};
