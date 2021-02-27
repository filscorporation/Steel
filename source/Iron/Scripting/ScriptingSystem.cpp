#include "ScriptingSystem.h"
#include "ScriptingCore.h"
#include "InternalCalls.h"
#include "../Core/Log.h"

MonoDomain* domain;
MonoImage* coreAssemblyImage;
MonoImage* customAssemblyImage;

MonoImage* LoadAssembly(const char* fileName)
{
    MonoAssembly* assembly = mono_domain_assembly_open(domain, fileName);
    MonoImage* assemblyImage = mono_assembly_get_image(assembly);

    return assemblyImage;
}

void ScriptingSystem::Init()
{
    Log::LogInfo("Begin scripting system init");

    mono_config_parse(nullptr);
    mono_set_dirs(R"(C:\Program Files (x86)\Mono\lib)",R"(C:\Program Files (x86)\Mono\etc)");

    domain = mono_jit_init("IronDomain");

    coreAssemblyImage = LoadAssembly("IronCore.dll");
    if (coreAssemblyImage == nullptr)
    {
        Log::LogError("Error loading core assembly");
        return;
    }
    customAssemblyImage = LoadAssembly("IronCustom.dll");
    if (customAssemblyImage == nullptr)
    {
        Log::LogError("Error loading custom assembly");
        return;
    }

    ScriptingCore::Init(coreAssemblyImage);

    Log::LogInfo("Scripting system initialized");
}

void ScriptingSystem::Terminate()
{
    ScriptingCore::Terminate();
    mono_jit_cleanup(domain);
}

void ScriptingSystem::CallEntryPoint()
{
    ScriptingCore::FindAndCallEntryPoint(customAssemblyImage);
}
