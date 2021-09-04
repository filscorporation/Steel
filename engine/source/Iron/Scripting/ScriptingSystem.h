#pragma once

#include "ScriptComponent.h"
#include "ScriptComponentSystem.h"

struct CachedData;

class ScriptingSystem
{
public:
    static void Init();
    static void Terminate();

    static void InitScene(EntitiesRegistry* entitiesRegistry);
    static void TerminateScene();

    static void CallEntryPoint(); // TODO: temp solution for entry point before scene serialization
    static void UpdateCoroutines();
    static bool IsInitialized();

private:
    static bool isInitialized;
    static ScriptComponentSystem* scriptComponentSystem;
};
