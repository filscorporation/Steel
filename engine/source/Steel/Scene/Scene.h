#pragma once

#include <list>

#include "HierarchyParent.h"
#include "IDComponent.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"
#include "Steel/Resources/AsepriteData.h"
#include "Steel/Rendering/SceneRenderer.h"
#include "Steel/UI/UILayer.h"

class UILayer;

class Scene : public HierarchyParent
{
public:
    EntitiesRegistry* GetEntitiesRegistry();
    UILayer* GetUILayer();

    explicit Scene(const std::string& name);
    ~Scene() override;
    void Init(bool initSystems);

    void CreateMainCamera();
    EntityID GetMainCamera();

    const std::string& GetName();
    void SetName(const std::string& name);

    EntityID CreateEntity();
    EntityID CreateEntity(const std::string& name, EntityID parent);
    EntityID CreateEntity(AsepriteData& data);
    EntityID CreateEmptyEntity();
    void DestroyEntity(EntityID entityID);
    void CleanDestroyedEntities();

    EntityID GetEntityByUUID(UUID_TYPE uuid);
    void SetEntityByUUID(UUID_TYPE uuid, EntityID entityID);

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
    std::string _name;
    bool systemsInitialized = false;

    std::unordered_map<UUID_TYPE, EntityID> entitiesByUUID;
    std::list<EntityID> entitiesToDelete;
    EntityID _mainCameraEntity = NULL_ENTITY;

    void SortByHierarchy();
    void UpdateGlobalTransformation();
    void RefreshTransformation();
    void CleanAllEntities();
    void DestroyAndRemoveEntity(EntityID entityID);
    void DestroyEntityInner(EntityID entityID);

    friend class SceneRenderer;
    friend class SerializationManager;
};
