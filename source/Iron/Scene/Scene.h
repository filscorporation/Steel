#pragma once

#include <vector>
#include "Entity.h"

class Scene
{
public:
    std::vector<Entity*> Entities; // TODO: private
    Camera* MainCamera;

    Scene();
    Entity* CreateEntity();
    Entity* GetEntity(uint64_t entityID);
    void DestroyEntity(Entity* entity);
    void CleanDestroyedEntities();
    void CleanAllEntities();

private:
    std::unordered_map<uint64_t, Entity*> entitiesByIDMap;
    std::vector<Entity*> entitiesToDelete;
    void DestroyAndRemoveEntity(Entity* entity);
    static void DestroyEntityInner(Entity* entity);
};