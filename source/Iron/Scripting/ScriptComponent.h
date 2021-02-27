#pragma once

#include "../Scene/Component.h"

struct ScriptMethods;

class ScriptComponent : public Component
{
public:
    void Init(const char* fullName, int64_t scriptPointer);

    void OnUpdate() override;
    void OnLateUpdate() override;

private:
    ScriptMethods* methods;
    const char* _fullName;
    int64_t _scriptPointer;
};
