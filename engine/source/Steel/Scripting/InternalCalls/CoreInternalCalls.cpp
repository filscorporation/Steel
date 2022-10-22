#include "CoreInternalCalls.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Core/Time.h"
#include "Steel/Input/Input.h"
#include "Steel/Physics/Physics.h"
#include "Steel/Math/Random.h"
#include "Steel/Math/PerlinNoise.h"

void CoreInternalCalls::Application_Quit()
{
    Application::Instance->Quit();
}

MonoString* CoreInternalCalls::Application_RuntimePath()
{
    return ScriptingCore::FromString(Application::Instance->GetRuntimePath().c_str());
}

MonoString* CoreInternalCalls::Application_DataPath()
{
    return ScriptingCore::FromString(Application::Instance->GetDataPath().c_str());
}

void CoreInternalCalls::Input_GetMousePosition(glm::vec2* position)
{
    auto temp = Input::GetMousePosition();
    position->x = temp.x;
    position->y = temp.y;
}

void CoreInternalCalls::Input_GetMouseScrollDelta(glm::vec2* delta)
{
    auto temp = Input::GetMouseScrollDelta();
    delta->x = temp.x;
    delta->y = temp.y;
}

bool CoreInternalCalls::Input_IsKeyPressed(int code)
{
    return Input::IsKeyPressed((KeyCodes::KeyCode)code);
}

bool CoreInternalCalls::Input_IsKeyJustPressed(int code)
{
    return Input::IsKeyJustPressed((KeyCodes::KeyCode)code);
}

bool CoreInternalCalls::Input_IsKeyJustReleased(int code)
{
    return Input::IsKeyJustReleased((KeyCodes::KeyCode)code);
}

bool CoreInternalCalls::Input_IsMousePressed(int code)
{
    return Input::IsMouseButtonPressed((MouseCodes::MouseCode)code);
}

bool CoreInternalCalls::Input_IsMouseJustPressed(int code)
{
    return Input::IsMouseButtonJustPressed((MouseCodes::MouseCode)code);
}

bool CoreInternalCalls::Input_IsMouseJustReleased(int code)
{
    return Input::IsMouseButtonJustReleased((MouseCodes::MouseCode)code);
}

ResourceID CoreInternalCalls::SceneManager_CreateNewScene(MonoString* name)
{
    auto sceneData = new SceneData(ScriptingCore::ToString(name));
    Application::Instance->GetResourcesManager()->AddResource(sceneData);
    return sceneData->ID;
}

void CoreInternalCalls::SceneManager_SetActiveScene(ResourceID sceneID)
{
    SceneData* sceneData = Application::Instance->GetResourcesManager()->GetSceneData(sceneID);
    if (sceneData == nullptr)
    {
        Log::LogError("Can't set active scene: scene {0} does not exist", sceneID);
        return;
    }

    auto scene = Application::Instance->GetSceneManager()->CreateNewScene(sceneData);
    if (!sceneData->Path.empty())
        SerializationManager::DeserializeScene(scene, sceneData->Path);

    Application::Instance->SwitchScenes(scene);
}

void CoreInternalCalls::SceneManager_SetActiveScene2(MonoString* path)
{
    const char* pathString = ScriptingCore::ToString(path);
    SceneData* sceneData = Application::Instance->GetResourcesManager()->LoadSceneData(pathString);
    if (sceneData == nullptr)
    {
        Log::LogError("Can't set active scene: file at {0} does not exist", pathString);
        return;
    }

    auto scene = Application::Instance->GetSceneManager()->CreateNewScene(sceneData);
    if (!sceneData->Path.empty())
        SerializationManager::DeserializeScene(scene, sceneData->Path);

    Application::Instance->SwitchScenes(scene);
}

ResourceID CoreInternalCalls::SceneManager_GetActiveScene()
{
    return Application::Instance->GetSceneManager()->GetActiveScene()->GetSourceID();
}

MonoString* CoreInternalCalls::Scene_GetName(ResourceID sceneID)
{
    SceneData* sceneData = Application::Instance->GetResourcesManager()->GetSceneData(sceneID);
    if (sceneData == nullptr)
    {
        Log::LogError("Scene {0} does not exist", sceneID);
        return nullptr;
    }
    return ScriptingCore::FromString(sceneData->GetName().c_str());
}

float CoreInternalCalls::Time_GetDeltaTime()
{
    return Time::DeltaTime();
}

float CoreInternalCalls::Time_GetUnscaledDeltaTime()
{
    return Time::UnscaledDeltaTime();
}

float CoreInternalCalls::Time_GetTimeSinceStartup()
{
    return Time::TimeSinceStartup();
}

uint64_t CoreInternalCalls::Time_GetFrameCount()
{
    return Time::FrameCount();
}

float CoreInternalCalls::Time_GetFixedDeltaTime()
{
    return Time::FixedDeltaTime;
}

void CoreInternalCalls::Time_SetFixedDeltaTime(float fixedDeltaTime)
{
    Time::FixedDeltaTime = std::max(fixedDeltaTime, 0.001f);
}

float CoreInternalCalls::Time_GetTimeScale()
{
    return Time::TimeScale;
}

void CoreInternalCalls::Time_SetTimeScale(float timeScale)
{
    Time::TimeScale = timeScale;
}

int CoreInternalCalls::Screen_GetWidth()
{
    return Screen::GetWidth();
}

void CoreInternalCalls::Screen_SetWidth(int width)
{
    Screen::SetWidth(width);
}

int CoreInternalCalls::Screen_GetHeight()
{
    return Screen::GetHeight();
}

void CoreInternalCalls::Screen_SetHeight(int height)
{
    Screen::SetHeight(height);
}

bool CoreInternalCalls::Screen_GetFullscreen()
{
    return Screen::GetFullscreen();
}

void CoreInternalCalls::Screen_SetFullscreen(bool fullscreen)
{
    Screen::SetFullscreen(fullscreen);
}

bool CoreInternalCalls::Screen_GetIsMinimized()
{
    return Screen::GetIsMinimized();
}

glm::vec4 CoreInternalCalls::Screen_GetColor()
{
    return glm::vec4(Screen::GetColor(), 1.0f);
}

void CoreInternalCalls::Screen_SetColor(glm::vec4 color)
{
    Screen::SetColor(color);
}

void CoreInternalCalls::Log_LogDebug(MonoString* message)
{
    Log::LogDebug(ScriptingCore::ToString(message));
}

void CoreInternalCalls::Log_LogInfo(MonoString* message)
{
    Log::LogInfo(ScriptingCore::ToString(message));
}

void CoreInternalCalls::Log_LogWarning(MonoString* message)
{
    Log::LogWarning(ScriptingCore::ToString(message));
}

void CoreInternalCalls::Log_LogError(MonoString* message)
{
    Log::LogError(ScriptingCore::ToString(message));
}

void CoreInternalCalls::Physics_Simulate(float deltaTime)
{
    Physics::Simulate(deltaTime);
}

MonoArray* CoreInternalCalls::Physics_PointCast(glm::vec2* point)
{
    return ScriptingCore::ToMonoUInt32Array(Physics::PointCast(*point));
}

MonoArray* CoreInternalCalls::Physics_AABBCast(glm::vec2* center, glm::vec2* size)
{
    return ScriptingCore::ToMonoUInt32Array(Physics::AABBCast(*center, *size));
}

MonoArray* CoreInternalCalls::Physics_LineCast(glm::vec2* pointA, glm::vec2* pointB)
{
    return ScriptingCore::ToMonoSimpleTypeArray<RayCastHit>(
            Physics::LineCast(*pointA, *pointB), SimpleAPITypes::RayCastHit);
}

float CoreInternalCalls::Random_NextFloat(float min, float max)
{
    return Random::NextFloat(min, max);
}

int CoreInternalCalls::Random_NextInt(int min, int max)
{
    return Random::NextInt(min, max);
}

float CoreInternalCalls::Random_PerlinNoise(float x, float y)
{
    return PerlinNoise::Get(x, y);
}
