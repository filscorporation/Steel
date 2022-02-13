#pragma once

#include <mono/jit/jit.h>

struct ScriptMethodPointer
{
    ScriptMethodPointer(MonoMethod* method, bool isInherited)
    {
        Method = method;
        IsInherited = isInherited;
    }

    MonoMethod* Method;
    bool IsInherited;
};
