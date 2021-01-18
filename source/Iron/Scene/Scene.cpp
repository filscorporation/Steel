#include "Scene.h"
#include "../Rendering/Camera.h"
#include "../Core/Application.h"
#include <algorithm>

Object* Scene::CreateObject()
{
    auto object = new Object();
    Objects.push_back(object);

    return object;
}

void Scene::DestroyObject(Object *object)
{
    objectsToDelete.push_back(object);
}

Scene::Scene()
{
    auto cameraObject = CreateObject();
    MainCamera = cameraObject->AddComponent<Camera>();
    cameraObject->Transform->Position = glm::vec3(0.0f, 0.0f, 3.0f);
}

void Scene::DestroyObjectInner(Object *object)
{
    // TODO: WIP, rework
    Objects.erase(std::remove(Objects.begin(), Objects.end(), object), Objects.end());
    delete object;
}

void Scene::CleanDestroyedObjects()
{
    for (auto object : objectsToDelete)
    {
        if (object != nullptr)
            DestroyObjectInner(object);
    }
    objectsToDelete.clear();
}

void Scene::CleanAllObjects()
{
    for (auto object : Objects)
    {
        DestroyObjectInner(object);
    }
    Objects.clear();
}
