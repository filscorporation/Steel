#pragma once

#include "ScriptComponent.h"

class ScriptingSystem
{
public:
    static void Init();
    static void Terminate();

    static void CreateDomain();
    static void UnloadDomain();
    static bool DomainLoaded();

    static void UpdateCoroutines();
    static bool IsInitialized();

private:
    static bool isInitialized;
};
