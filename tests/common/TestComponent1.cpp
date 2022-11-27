#include "TestComponent1.h"
#include "Steel/Serialization/AttributesRegistration.h"

void TestComponent1::RegisterType()
{
    REGISTER_COMPONENT(TestComponent1);
}

void TestComponent1::OnCreated(EntitiesRegistry *entitiesRegistry)
{
    Component::OnCreated(entitiesRegistry);

    IntegerValue = 7;
}

void TestComponent1::OnRemoved(EntitiesRegistry *entitiesRegistry)
{
    Component::OnRemoved(entitiesRegistry);
}

void TestComponent1::OnEnabled(EntitiesRegistry *entitiesRegistry)
{
    Component::OnEnabled(entitiesRegistry);

    IntegerValue = 11;
}

void TestComponent1::OnDisabled(EntitiesRegistry *entitiesRegistry)
{
    Component::OnDisabled(entitiesRegistry);

    IntegerValue = 9;
}
