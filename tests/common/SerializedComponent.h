#pragma once

#include "Steel/EntityComponentSystem/Component.h"

class SerializedComponent : public Component
{
    COMPONENT(SerializedComponent)

public:
    explicit SerializedComponent(EntityID ownerEntityID) : Component(ownerEntityID) { }

public:
    static void RegisterType();

    int GetIntegerAttribute() const;
    float GetFloatAttribute() const;
    bool GetBoolAttribute() const;
    EntityID GetEntityIDAttribute() const;
    const std::string& GetStringAttribute() const;
    const glm::vec2& GetVec2Attribute() const;
    const glm::vec3& GetVec3Attribute() const;
    const glm::vec4& GetVec4Attribute() const;

    void SetIntegerAttribute(int newValue);
    void SetFloatAttribute(float newValue);
    void SetBoolAttribute(bool newValue);
    void SetEntityIDAttribute(EntityID newValue);
    void SetStringAttribute(const std::string& newValue);
    void SetVec2Attribute(const glm::vec2& newValue);
    void SetVec3Attribute(const glm::vec3& newValue);
    void SetVec4Attribute(const glm::vec4& newValue);

    int IntegerAttribute = 0;
    float FloatAttribute = 0.0f;
    bool BoolAttribute = false;
    EntityID EntityIDAttribute = NULL_ENTITY;
    std::string StringAttribute;
    glm::vec2 Vec2Attribute;
    glm::vec3 Vec3Attribute;
    glm::vec4 Vec4Attribute;
};
