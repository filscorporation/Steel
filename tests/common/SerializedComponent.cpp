#include "SerializedComponent.h"
#include "Steel/Serialization/AttributesRegistration.h"

void SerializedComponent::RegisterType()
{
    REGISTER_COMPONENT(SerializedComponent);
    REGISTER_ATTRIBUTE(SerializedComponent, "integerAttribute", GetIntegerAttribute, SetIntegerAttribute, int, AttributeFlags::Public, "integerAttribute");
    REGISTER_ATTRIBUTE(SerializedComponent, "floatAttribute", GetFloatAttribute, SetFloatAttribute, float, AttributeFlags::Public, "floatAttribute");
    REGISTER_ATTRIBUTE(SerializedComponent, "boolAttribute", GetBoolAttribute, SetBoolAttribute, bool, AttributeFlags::Public, "boolAttribute");
    REGISTER_ATTRIBUTE(SerializedComponent, "entityIDAttribute", GetEntityIDAttribute, SetEntityIDAttribute, EntityID, AttributeFlags::Public, "entityIDAttribute");
    REGISTER_ATTRIBUTE(SerializedComponent, "stringAttribute", GetStringAttribute, SetStringAttribute, std::string, AttributeFlags::Public, "stringAttribute");
    REGISTER_ATTRIBUTE(SerializedComponent, "vec2Attribute", GetVec2Attribute, SetVec2Attribute, glm::vec2, AttributeFlags::Public, "vec2Attribute");
    REGISTER_ATTRIBUTE(SerializedComponent, "vec3Attribute", GetVec3Attribute, SetVec3Attribute, glm::vec3, AttributeFlags::Public, "vec3Attribute");
    REGISTER_ATTRIBUTE(SerializedComponent, "vec4Attribute", GetVec4Attribute, SetVec4Attribute, glm::vec4, AttributeFlags::Public, "vec4Attribute");
    REGISTER_LIST_ATTRIBUTE(SerializedComponent, "testTypeListAttribute", GetTestTypeListAttribute, SetTestTypeListAttribute, TestType, AttributeFlags::Public, "testTypeListAttribute");
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

const std::string& SerializedComponent::GetStringAttribute() const
{
    return StringAttribute;
}

const glm::vec2& SerializedComponent::GetVec2Attribute() const
{
    return Vec2Attribute;
}

const glm::vec3& SerializedComponent::GetVec3Attribute() const
{
    return Vec3Attribute;
}

const glm::vec4& SerializedComponent::GetVec4Attribute() const
{
    return Vec4Attribute;
}

const std::vector<TestType>& SerializedComponent::GetTestTypeListAttribute() const
{
    return TestTypeListAttribute;
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

void SerializedComponent::SetStringAttribute(const std::string& newValue)
{
    StringAttribute = newValue;
}

void SerializedComponent::SetVec2Attribute(const glm::vec2& newValue)
{
    Vec2Attribute = newValue;
}

void SerializedComponent::SetVec3Attribute(const glm::vec3& newValue)
{
    Vec3Attribute = newValue;
}

void SerializedComponent::SetVec4Attribute(const glm::vec4& newValue)
{
    Vec4Attribute = newValue;
}

void SerializedComponent::SetTestTypeListAttribute(const std::vector<TestType>& newValue)
{
    TestTypeListAttribute = newValue;
}
