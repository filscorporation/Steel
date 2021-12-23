#include "IDComponent.h"

void IDComponent::RegisterType()
{
    REGISTER_TYPE(IDComponent);
    REGISTER_ATTRIBUTE(IDComponent, "uuid", GetUUID, SetUUID, UUID, AttributeFlags::Public);
}

UUID IDComponent::GetUUID() const
{
    return _uuid;
}

void IDComponent::SetUUID(UUID uuid)
{
    _uuid = uuid;
}
