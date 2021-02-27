#pragma once

#include <cstdint>

namespace InternalCalls
{
    // Core
    void Application_Quit();
    bool Input_IsKeyPressed(int code);

    //Object
    uint64_t Entity_CreateNewEntity();
    uint64_t Entity_AddComponent(uint64_t id, void* type);
    uint64_t Entity_AddScriptComponent(uint64_t id, void* type, int64_t scriptPointer);
    uint64_t Entity_GetComponent(uint64_t id, void* type);
    uint64_t Entity_GetScriptComponent(uint64_t id, void* type);
    uint64_t Entity_RemoveComponent(uint64_t id, void* type);
    uint64_t Entity_RemoveScriptComponent(uint64_t id, void* type);
}