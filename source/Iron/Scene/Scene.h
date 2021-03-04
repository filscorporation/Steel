#pragma once

#include <list>
#include "Entity.h"

class Scene
{
public:
    std::list<Entity*> Entities; // TODO: private

    Scene();
    Entity* CreateEntity();
    Entity* GetEntity(uint64_t entityID);
    void DestroyEntity(Entity* entity);
    void CleanDestroyedEntities();
    void CleanAllEntities();

    Camera* GetMainCamera();

    static int EntitiesWasCreated;

private:
    std::unordered_map<uint64_t, Entity*> entitiesByIDMap;
    std::list<Entity*> entitiesToDelete;
    Camera* _mainCamera;
    void DestroyAndRemoveEntity(Entity* entity);
    static void DestroyEntityInner(Entity* entity);
};