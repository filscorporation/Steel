#include "Scene.h"
#include "Transform.h"
#include "Log.h"
#include "Application.h"

Object* Scene::CreateObject()
{
    auto object = new Object();
    object->AddComponent<Transform>();
    Objects.push_back(object);
    Log::LogInfo(std::to_string(Application::Instance->GetCurrentScene()->Objects.size()).c_str());
    Log::LogInfo(std::to_string(Objects.size()).c_str());

    return object;
}
