#include "IDComponent.h"
#include "Steel/Serialization/AttributesRegistration.h"

void IDComponent::RegisterType()
{
    REGISTER_COMPONENT(IDComponent);
    REGISTER_ATTRIBUTE(IDComponent, "uuid", GetUUID, SetUUID, UUID_TYPE, AttributeFlags::Public, "UUID");
}

UUID_TYPE IDComponent::GetUUID() const
{
    return _uuid;
}

void IDComponent::SetUUID(UUID_TYPE uuid)
{
    _uuid = uuid;
}
