#pragma once

#include <vector>
#include "Object.h"

class Scene
{
public:
    std::vector<Object*> Objects;
    Camera* MainCamera;

    Scene();
    Object* CreateObject();
    void DestroyObject(Object* object);
    void CleanDestroyedObjects();
    void CleanAllObjects();

private:
    std::vector<Object*> objectsToDelete;
    void DestroyAndRemoveObject(Object* object);
    static void DestroyObjectInner(Object* object);
};