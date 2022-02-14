#pragma once

#include <mono/jit/jit.h>

class ScriptObjectHandler
{
public:
    explicit ScriptObjectHandler(MonoObject* monoObject);
    ~ScriptObjectHandler();

    MonoObject* GetMonoObject() const;

private:
    uint32_t _gcHandle;
};
