#include "Scene.h"
#include "../Rendering/Camera.h"
#include <algorithm>

Object* Scene::CreateObject()
{
    auto object = new Object();
    Objects.push_back(object);

    return object;
}

void Scene::DestroyObject(Object *object)
{
    // TODO: WIP, rework
    Objects.erase(std::remove(Objects.begin(), Objects.end(), object), Objects.end());
    free(object);
}

Scene::Scene()
{
    auto cameraObject = CreateObject();
    MainCamera = cameraObject->AddComponent<Camera>();
    cameraObject->Transform->Position = glm::vec3(0.0f, 0.0f, 3.0f);
}
