#pragma once

#include <mono/jit/jit.h>

class ScriptPointer
{
public:
    explicit ScriptPointer(MonoObject* monoObject);
    ~ScriptPointer();

    MonoObject* GetMonoObject() const;

private:
    uint32_t _gcHandle;
};
