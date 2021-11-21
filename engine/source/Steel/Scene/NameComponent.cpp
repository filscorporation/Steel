#include "NameComponent.h"

void NameComponent::RegisterType()
{
    REGISTER_TYPE(NameComponent);
    REGISTER_ATTRIBUTE(NameComponent, "name", GetName, SetName, std::string, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(NameComponent, "tag", GetTag, SetTag, std::string, AttributeFlags::Public);
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
