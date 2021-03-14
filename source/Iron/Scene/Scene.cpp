#include <algorithm>

#include "Scene.h"
#include "Transformation.h"
#include "NameComponent.h"

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
    EntitiesWasCreated++;

    auto entity = entitiesRegistry->CreateNewEntity();
    auto& name = entitiesRegistry->AddComponent<NameComponent>(entity);
    name.Name = "New entity";
    entitiesRegistry->AddComponent<Transformation>(entity);

    return entity;
}

void Scene::DestroyEntity(EntityID entity)
{
    entitiesToDelete.push_back(entity);
}

bool Scene::IsEntityDestroyed(EntityID entity)
{
    // TODO: too slow
    if(std::find(entitiesToDelete.begin(), entitiesToDelete.end(), entity) != entitiesToDelete.end())
        return true;
    else
        return false;
}

void Scene::DestroyAndRemoveEntity(EntityID entity)
{
    DestroyEntityInner(entity);
}

void Scene::DestroyEntityInner(EntityID entity)
{
    entitiesRegistry->DeleteEntity(entity);
}

void Scene::CleanDestroyedEntities()
{
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
