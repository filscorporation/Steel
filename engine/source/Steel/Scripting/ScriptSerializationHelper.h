#pragma once

#include "ScriptingCommon.h"

#include <string>

class ScriptSerializationHelper
{
public:
    static bool RestoreScriptInstance(const std::string& classNamespace, const std::string& className, ScriptData& scriptData);
};
