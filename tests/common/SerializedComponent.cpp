#include "SerializedComponent.h"

bool registered = false;

void SerializedComponent::RegisterType()
{
    if (registered)
        return;
    registered = true;

    REGISTER_ATTRIBUTE(SerializedComponent, "integerAttribute", GetIntegerAttribute, SetIntegerAttribute, int, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(SerializedComponent, "floatAttribute", GetFloatAttribute, SetFloatAttribute, float, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(SerializedComponent, "boolAttribute", GetBoolAttribute, SetBoolAttribute, bool, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(SerializedComponent, "entityIDAttribute", GetEntityIDAttribute, SetEntityIDAttribute, EntityID, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(SerializedComponent, "stringAttribute", GetStringAttribute, SetStringAttribute, std::string, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(SerializedComponent, "vec2Attribute", GetVec2Attribute, SetVec2Attribute, glm::vec2, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(SerializedComponent, "vec3Attribute", GetVec3Attribute, SetVec3Attribute, glm::vec3, AttributeFlags::Public);
    REGISTER_ATTRIBUTE(SerializedComponent, "vec4Attribute", GetVec4Attribute, SetVec4Attribute, glm::vec4, AttributeFlags::Public);
}

int SerializedComponent::GetIntegerAttribute() const
{
    return IntegerAttribute;
}

float SerializedComponent::GetFloatAttribute() const
{
    return FloatAttribute;
}

bool SerializedComponent::GetBoolAttribute() const
{
    return BoolAttribute;
}

EntityID SerializedComponent::GetEntityIDAttribute() const
{
    return EntityIDAttribute;
}

std::string SerializedComponent::GetStringAttribute() const
{
    return StringAttribute;
}

glm::vec2 SerializedComponent::GetVec2Attribute() const
{
    return Vec2Attribute;
}

glm::vec3 SerializedComponent::GetVec3Attribute() const
{
    return Vec3Attribute;
}

glm::vec4 SerializedComponent::GetVec4Attribute() const
{
    return Vec4Attribute;
}

void SerializedComponent::SetIntegerAttribute(int newValue)
{
    IntegerAttribute = newValue;
}

void SerializedComponent::SetFloatAttribute(float newValue)
{
    FloatAttribute = newValue;
}

void SerializedComponent::SetBoolAttribute(bool newValue)
{
    BoolAttribute = newValue;
}

void SerializedComponent::SetEntityIDAttribute(EntityID newValue)
{
    EntityIDAttribute = newValue;
}

void SerializedComponent::SetStringAttribute(std::string newValue)
{
    StringAttribute = newValue;
}

void SerializedComponent::SetVec2Attribute(glm::vec2 newValue)
{
    Vec2Attribute = newValue;
}

void SerializedComponent::SetVec3Attribute(glm::vec3 newValue)
{
    Vec3Attribute = newValue;
}

void SerializedComponent::SetVec4Attribute(glm::vec4 newValue)
{
    Vec4Attribute = newValue;
}
