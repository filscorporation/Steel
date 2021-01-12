#include "Scene.h"
#include "../Rendering/Camera.h"

Object* Scene::CreateObject()
{
    auto object = new Object();
    Objects.push_back(object);

    return object;
}

Scene::Scene()
{
    auto cameraObject = CreateObject();
    MainCamera = cameraObject->AddComponent<Camera>();
    cameraObject->Transform->Position = glm::vec3(0.0f, 0.0f, 3.0f);
}
