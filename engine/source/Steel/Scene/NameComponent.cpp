#include "NameComponent.h"
#include "Steel/Serialization/AttributesRegistration.h"

void NameComponent::RegisterType()
{
    REGISTER_COMPONENT(NameComponent);
    REGISTER_ATTRIBUTE(NameComponent, "name", GetName, SetName, std::string, AttributeFlags::Public, "Name");
    REGISTER_ATTRIBUTE(NameComponent, "tag", GetTag, SetTag, std::string, AttributeFlags::Public, "Tag");
}

const std::string& NameComponent::GetName() const
{
    return _name;
}

void NameComponent::SetName(const std::string& name)
{
    _name = name;
}

const std::string& NameComponent::GetTag() const
{
    return _tag;
}

void NameComponent::SetTag(const std::string& tag)
{
    _tag = tag;
}
