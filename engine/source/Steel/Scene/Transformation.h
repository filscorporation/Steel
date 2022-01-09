#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "HierarchyNode.h"
#include "Steel/EntityComponentSystem/Component.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"

class Transformation : public Component
{
    COMPONENT(Transformation)

public:
    explicit Transformation(EntityID ownerEntityID) : Component(ownerEntityID) { };

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;

    const glm::vec3& GetPosition();
    void SetPosition(const glm::vec3& position);
    const glm::vec3& GetRotation();
    void SetRotation(const glm::vec3& rotation);
    const glm::vec3& GetScale();
    void SetScale(const glm::vec3& scale);

    const glm::vec3& GetLocalPosition() const;
    void SetLocalPosition(const glm::vec3& position);
    const glm::vec3& GetLocalRotation() const;
    void SetLocalRotation(const glm::vec3& rotation);
    const glm::vec3& GetLocalScale() const;
    void SetLocalScale(const glm::vec3& scale);

    virtual const glm::mat4& GetTransformationMatrixCached();
    virtual void UpdateTransformation(ComponentAccessor<Transformation>& transformationsAccessor, HierarchyNode& hierarchyNode);
    virtual const glm::mat4& GetTransformationMatrix();
    virtual const glm::mat4& GetInverseTransformationMatrix();
    float GetGlobalSortingOrderCached() const;

    // Did anything in transformation change in this frame
    bool DidTransformationChange() const;
    void SetTransformationChanged(bool changed);
    void RefreshTransformation();

private:
    glm::mat4 _transformationMatrix = glm::mat4(1.0f);
    glm::mat4 _inverseTransformationMatrix = glm::mat4(1.0f);

    glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _scale = { 1.0f, 1.0f, 1.0f };
    glm::vec3 _localPosition = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _localRotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _localScale = { 1.0f, 1.0f, 1.0f };

    bool transformationChanged = true;
};
