#include "TestType.h"

void TestType::RegisterType()
{
    REGISTER_TYPE(TestType);
    REGISTER_ATTRIBUTE(TestType, "value", GetValue, SetValue, int, AttributeFlags::Public);
}

TestType::TestType(int value)
{
    _value = value;
}

int TestType::GetValue() const
{
    return _value;
}

void TestType::SetValue(int newValue)
{
    _value = newValue;
}
