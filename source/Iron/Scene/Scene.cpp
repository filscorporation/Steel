#include "Scene.h"
#include "../Rendering/Camera.h"
#include <algorithm>

Entity* Scene::CreateEntity()
{
    auto entity = new Entity();
    Entities.push_back(entity);
    entitiesByIDMap[entity->ID] = entity;

    return entity;
}

void Scene::DestroyEntity(Entity *entity)
{
    entitiesToDelete.push_back(entity);
}

Scene::Scene()
{
    auto cameraEntity = CreateEntity();
    MainCamera = cameraEntity->AddComponent<Camera>();
    cameraEntity->Transform->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    MainCamera->SetHeight(3.0f);
}

void Scene::DestroyAndRemoveEntity(Entity *entity)
{
    // TODO: WIP, rework (removing is very slow, up to 35% of CPU on many objects tests)
    Entities.erase(std::remove(Entities.begin(), Entities.end(), entity), Entities.end());
    entitiesByIDMap[entity->ID] = nullptr;
    DestroyEntityInner(entity);
}

void Scene::DestroyEntityInner(Entity *entity)
{
    delete entity;
}

void Scene::CleanDestroyedEntities()
{
    for (auto entity : entitiesToDelete)
    {
        if (entity != nullptr)
            DestroyAndRemoveEntity(entity);
    }
    entitiesToDelete.clear();
}

void Scene::CleanAllEntities()
{
    for (auto entity : Entities)
    {
        DestroyEntityInner(entity);
    }
}

Entity *Scene::GetEntity(uint64_t entityID)
{
    return entitiesByIDMap[entityID];
}
