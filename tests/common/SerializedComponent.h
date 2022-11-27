#pragma once

#include "Steel/EntityComponentSystem/Component.h"
#include "TestType.h"

#include <glm/glm.hpp>

class SerializedComponent : public Component
{
    DEFINE_TYPE(SerializedComponent)

public:
    explicit SerializedComponent(EntityID ownerEntityID) : Component(ownerEntityID) { }

public:
    int GetIntegerAttribute() const;
    float GetFloatAttribute() const;
    bool GetBoolAttribute() const;
    EntityID GetEntityIDAttribute() const;
    const std::string& GetStringAttribute() const;
    const glm::vec2& GetVec2Attribute() const;
    const glm::vec3& GetVec3Attribute() const;
    const glm::vec4& GetVec4Attribute() const;
    const std::vector<TestType>& GetTestTypeListAttribute() const;

    void SetIntegerAttribute(int newValue);
    void SetFloatAttribute(float newValue);
    void SetBoolAttribute(bool newValue);
    void SetEntityIDAttribute(EntityID newValue);
    void SetStringAttribute(const std::string& newValue);
    void SetVec2Attribute(const glm::vec2& newValue);
    void SetVec3Attribute(const glm::vec3& newValue);
    void SetVec4Attribute(const glm::vec4& newValue);
    void SetTestTypeListAttribute(const std::vector<TestType>& newValue);

    int IntegerAttribute = 0;
    float FloatAttribute = 0.0f;
    bool BoolAttribute = false;
    EntityID EntityIDAttribute = NULL_ENTITY;
    std::string StringAttribute;
    glm::vec2 Vec2Attribute;
    glm::vec3 Vec3Attribute;
    glm::vec4 Vec4Attribute;
    std::vector<TestType> TestTypeListAttribute;
};
