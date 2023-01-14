#include "ScriptingSystem.h"
#include "ScriptingCore.h"
#include "Steel/Core/Log.h"

// TODO: temp solution for development, later should link to Steel bins folder
#if PLATFORM_WINDOWS
#define DEBUG_MONO_LIB_PATH "C:\\Program Files (x86)\\Mono\\lib"
#define DEBUG_MONO_ETC_PATH "C:\\Program Files (x86)\\Mono\\etc"
#define DEBUG_API_DLL_PATH "..\\..\\..\\scripting\\SteelCustom\\bin\\Debug\\SteelCore.dll"
#define DEBUG_SCRIPTS_DLL_PATH "..\\..\\..\\scripting\\SteelCustom\\bin\\Debug\\SteelCustom.dll"
#define DISTRIBUTE_MONO_LIB_PATH "Mono\\lib"
#define DISTRIBUTE_MONO_ETC_PATH "Mono\\etc"
#define DISTRIBUTE_API_DLL_PATH "Bin\\SteelCore.dll"
#define DISTRIBUTE_SCRIPTS_DLL_PATH "Bin\\SteelCustom.dll"
#endif
#ifdef PLATFORM_LINUX
#define DEBUG_MONO_LIB_PATH "/usr/lib/"
#define DEBUG_MONO_ETC_PATH "/usr/lib/mono"
#define DEBUG_API_DLL_PATH "../../../scripting/SteelCustom/bin/Debug/SteelCore.dll"
#define DEBUG_SCRIPTS_DLL_PATH "../../../scripting/SteelCustom/bin/Debug/SteelCustom.dll"
#endif

bool ScriptingSystem::isInitialized = false;

void ScriptingSystem::Init()
{
    if (isInitialized)
    {
        Log::LogWarning("Scripting system is already initialized");
        return;
    }

#ifdef DISTRIBUTE_BUILD
    isInitialized = ScriptingCore::Init(DISTRIBUTE_MONO_LIB_PATH, DISTRIBUTE_MONO_ETC_PATH);
#else
    isInitialized = ScriptingCore::Init(DEBUG_MONO_LIB_PATH, DEBUG_MONO_ETC_PATH);
#endif

    if (isInitialized)
        Log::LogDebug("Scripting system initialized");
}

void ScriptingSystem::Terminate()
{
    if (!isInitialized)
        return;

    ScriptingCore::Terminate();
    isInitialized = false;
}

void ScriptingSystem::CreateDomain()
{
#ifdef DISTRIBUTE_BUILD
    ScriptingCore::CreateDomain(DISTRIBUTE_API_DLL_PATH, DISTRIBUTE_SCRIPTS_DLL_PATH);
#else
    ScriptingCore::CreateDomain(DEBUG_API_DLL_PATH, DEBUG_SCRIPTS_DLL_PATH);
#endif
}

void ScriptingSystem::UnloadDomain()
{
    ScriptingCore::UnloadDomain();
}

bool ScriptingSystem::DomainLoaded()
{
    return ScriptingCore::DomainLoaded();
}

void ScriptingSystem::UpdateCoroutines()
{
    if (IsInitialized())
        ScriptingCore::CallMethod(ScriptingCore::GetCoroutinesUpdateMethod(), nullptr, nullptr);
}

bool ScriptingSystem::IsInitialized()
{
    return isInitialized && DomainLoaded();
}
