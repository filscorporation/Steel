#include <algorithm>

#include "Scene.h"
#include "Transformation.h"
#include "NameComponent.h"
#include "HierarchyNode.h"
#include "Hierarchy.h"

int Scene::EntitiesWasCreated = 0;

Scene::Scene()
{
    entitiesRegistry = new EntitiesRegistry();
}

Scene::~Scene()
{
    delete entitiesRegistry;
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

EntityID Scene::CreateEntity()
{
    return CreateEntity("New entity", NULL_ENTITY);
}

EntityID Scene::CreateEntity(const char* name, EntityID parent)
{
    EntitiesWasCreated++;

    auto entity = entitiesRegistry->CreateNewEntity();
    auto& nameComponent = entitiesRegistry->AddComponent<NameComponent>(entity);
    nameComponent.Name = name;
    entitiesRegistry->AddComponent<Transformation>(entity);
    entitiesRegistry->AddComponent<HierarchyNode>(entity);
    LinkChildToParent(entitiesRegistry, entity, parent);

    return entity;
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
}

void Scene::CleanAllEntities()
{
    entitiesRegistry->CleanAllEntities();
}

Camera& Scene::GetMainCamera()
{
    return entitiesRegistry->GetComponent<Camera>(_mainCameraEntity);
}
