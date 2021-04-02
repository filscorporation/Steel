#pragma once

#include "ScriptComponent.h"

struct CachedData;

class ScriptingSystem
{
public:
    static void Init();
    static void Terminate();

    static void CallEntryPoint(); // TODO: temp solution for entry point before scene serialization
    static bool IsInitialized();

private:
    static bool isInitialized;
};
