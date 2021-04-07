#include "TestComponent1System.h"

void TestComponent1System::OnComponentAdded(EntityID entityID, TestComponent1& component)
{
    ++CatchAdded;
    component.IntegerValue = 7;
}

void TestComponent1System::OnComponentRemoved(EntityID entityID, TestComponent1& component)
{
    ++CatchRemoved;
}

void TestComponent1System::OnEntityEnabled(EntityID entityID, TestComponent1& component)
{
    ++CatchEnabled;
    component.IntegerValue = 11;
}

void TestComponent1System::OnEntityDisabled(EntityID entityID, TestComponent1& component)
{
    ++CatchDisabled;
    component.IntegerValue = 9;
}
