#include <algorithm>

#include "Scene.h"
#include "Transformation.h"
#include "NameComponent.h"
#include "Hierarchy.h"

int Scene::EntitiesWasCreated = 0;

Scene::Scene()
{
    hierarchySystem = new HierarchySystem();
    transformationSystem = new TransformationSystem();
    entitiesRegistry = new EntitiesRegistry();
    uiLayer = new UILayer(this);
    entitiesRegistry->RegisterSystem<HierarchyNode>(hierarchySystem);
    entitiesRegistry->RegisterSystem<Transformation>(transformationSystem);
    entitiesRegistry->RegisterSystem<RectTransformation>(transformationSystem);
}

Scene::~Scene()
{
    delete uiLayer;
    delete entitiesRegistry;
    delete transformationSystem;
    delete hierarchySystem;
}

void Scene::CreateMainCamera()
{
    _mainCameraEntity = CreateEntity();
    auto& mainCamera = entitiesRegistry->AddComponent<Camera>(_mainCameraEntity);
    entitiesRegistry->GetComponent<Transformation>(_mainCameraEntity).SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    mainCamera.SetHeight(3.0f);
}

EntitiesRegistry* Scene::GetEntitiesRegistry()
{
    return entitiesRegistry;
}

UILayer* Scene::GetUILayer()
{
    return uiLayer;
}

EntityID Scene::CreateEntity()
{
    return CreateEntity("New entity", NULL_ENTITY);
}

EntityID Scene::CreateEntity(const char* name, EntityID parent)
{
    auto entity = CreateEmptyEntity();
    auto& nameComponent = entitiesRegistry->AddComponent<NameComponent>(entity);
    nameComponent.Name = name;
    entitiesRegistry->AddComponent<Transformation>(entity);
    entitiesRegistry->AddComponent<HierarchyNode>(entity);
    LinkChildToParent(entitiesRegistry, entity, parent);

    return entity;
}

EntityID Scene::CreateEmptyEntity()
{
    EntitiesWasCreated++;

    return entitiesRegistry->CreateNewEntity();
}

void Scene::DestroyEntity(EntityID entity)
{
    entitiesToDelete.push_back(entity);
}

void Scene::DestroyAndRemoveEntity(EntityID entity)
{
    DestroyEntityInner(entity);
}

void Scene::DestroyEntityInner(EntityID entity)
{
    entitiesRegistry->DeleteEntity(entity);
}

void Scene::SortByHierarchy()
{
    // Sort all objects by hierarchy depth
    struct
    {
        bool operator()(const HierarchyNode& a, const HierarchyNode& b) const { return a.HierarchyDepth < b.HierarchyDepth; }
    } DepthComparer;
    entitiesRegistry->SortComponents<HierarchyNode>(DepthComparer);
}

void Scene::UpdateGlobalTransformation()
{
    auto hierarchyNodes = entitiesRegistry->GetComponentIterator<HierarchyNode>();
    auto transformationsAccessor = entitiesRegistry->GetComponentAccessor<Transformation>();
    for (auto& hierarchyNode : hierarchyNodes)
    {
        if (transformationsAccessor.Has(hierarchyNode.Owner))
            transformationsAccessor.Get(hierarchyNode.Owner).UpdateTransformation(transformationsAccessor, hierarchyNode);
    }
}

void Scene::CleanDestroyedEntities()
{
    // TODO: destroy entity's children
    for (auto entity : entitiesToDelete)
    {
        if (entity != NULL_ENTITY)
            DestroyAndRemoveEntity(entity);
    }
    entitiesToDelete.clear();

    entitiesRegistry->ClearRemoved();
}

void Scene::CleanAllEntities()
{
    entitiesRegistry->CleanAllEntities();
}

Camera& Scene::GetMainCamera()
{
    return entitiesRegistry->GetComponent<Camera>(_mainCameraEntity);
}
