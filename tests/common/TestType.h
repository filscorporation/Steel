#pragma once

#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/Serialization/Serializable.h"

struct TestType : public Serializable
{
    DEFINE_TYPE(TestType)

    TestType() = default;
    TestType(int value);

    int GetValue() const;
    void SetValue(int newValue);

private:
    int _value = -1;
};
