#pragma once

#include "Steel/Common/FlagType.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace ScriptEventTypes
{
    enum ScriptEventType
    {
        OnUpdate              = 1 << 0,
        OnCreate              = 1 << 1,
        OnDestroy             = 1 << 2,
        OnFixedUpdate         = 1 << 3,
        OnLateUpdate          = 1 << 4,
        OnEnabled             = 1 << 5,
        OnDisabled            = 1 << 6,
        OnCollisionEnter      = 1 << 7,
        OnCollisionStay       = 1 << 8,
        OnCollisionExit       = 1 << 9,
        OnMouseOver           = 1 << 10,
        OnMouseEnter          = 1 << 11,
        OnMouseExit           = 1 << 12,
        OnMousePressed        = 1 << 13,
        OnMouseJustPressed    = 1 << 14,
        OnMouseJustReleased   = 1 << 15,
        OnMouseOverUI         = 1 << 16,
        OnMouseEnterUI        = 1 << 17,
        OnMouseExitUI         = 1 << 18,
        OnMousePressedUI      = 1 << 19,
        OnMouseJustPressedUI  = 1 << 20,
        OnMouseJustReleasedUI = 1 << 21,
    };

    FLAG_TYPE(ScriptEventType)
}

class ScriptAttributeAccessorBase;

struct ScriptAttributeInfo
{
    std::string FieldName;
    ScriptAttributeAccessorBase* Accessor;
};

struct ScriptMethodPointer;

struct ScriptTypeInfo
{
    std::string TypeName;
    std::string TypeNamespace;
    ScriptEventTypes::ScriptEventType Mask;
    std::vector<ScriptAttributeInfo> Attributes;
    std::unordered_map<ScriptEventTypes::ScriptEventType, ScriptMethodPointer*> EventMethods;
};

class ScriptObjectHandler;

struct ScriptData
{
    ScriptObjectHandler* ScriptHandler;
    ScriptTypeInfo* TypeInfo;
};
