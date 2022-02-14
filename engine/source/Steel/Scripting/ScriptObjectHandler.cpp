#include "ScriptObjectHandler.h"

ScriptObjectHandler::ScriptObjectHandler(MonoObject* monoObject)
{
    _gcHandle = mono_gchandle_new(monoObject, false);
}

ScriptObjectHandler::~ScriptObjectHandler()
{
    if (_gcHandle != 0)
    {
        mono_gchandle_free(_gcHandle);
        _gcHandle = 0;
    }
}

MonoObject* ScriptObjectHandler::GetMonoObject() const
{
    return mono_gchandle_get_target(_gcHandle);
}
