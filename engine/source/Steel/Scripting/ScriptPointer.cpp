#include "ScriptPointer.h"

ScriptPointer::ScriptPointer(MonoObject* monoObject)
{
    _gcHandle = mono_gchandle_new(monoObject, false);
}

ScriptPointer::~ScriptPointer()
{
    if (_gcHandle != 0)
    {
        mono_gchandle_free(_gcHandle);
        _gcHandle = 0;
    }
}

MonoObject* ScriptPointer::GetMonoObject() const
{
    return mono_gchandle_get_target(_gcHandle);
}
