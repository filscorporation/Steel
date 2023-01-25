#include "ScriptSerializationHelper.h"
#include "ScriptingSystem.h"
#include "ScriptingCore.h"

bool ScriptSerializationHelper::RestoreScriptInstance(const std::string& classNamespace, const std::string& className, ScriptData& scriptData)
{
    if (!ScriptingSystem::IsInitialized())
        return false;

    MonoClass* monoClass = ScriptingCore::GetMonoClassByFullName(classNamespace, className);
    if (monoClass == nullptr)
    {
        Log::LogError("Couldn't find mono class {0}.{1}", classNamespace, className);
        return false;
    }

    ScriptTypeInfo* typeInfo = ScriptingCore::ScriptParseRecursive(monoClass);
    ScriptObjectHandler* handler = ScriptingCore::CreateManagedInstance(monoClass, nullptr, 0);
    if (handler == nullptr)
    {
        Log::LogError("Couldn't create instance of class {0}.{1}", classNamespace, className);
        return false;
    }

    scriptData.ScriptHandler = handler;
    scriptData.TypeInfo = typeInfo;

    return true;
}
