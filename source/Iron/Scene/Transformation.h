#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "../EntityComponentSystem/Component.h"
#include "../EntityComponentSystem/EntitiesRegistry.h"

class Transformation : public Component
{
public:
    explicit Transformation(EntityID ownerEntityID) : Component(ownerEntityID) { }

    virtual glm::vec3 GetPosition();
    virtual void SetPosition(const glm::vec3& position);
    glm::vec3 GetRotation();
    void SetRotation(const glm::vec3& rotation);
    glm::vec3 GetScale();
    void SetScale(const glm::vec3& scale);

    virtual glm::vec3 GetLocalPosition() const;
    virtual void SetLocalPosition(const glm::vec3& position);
    glm::vec3 GetLocalRotation() const;
    void SetLocalRotation(const glm::vec3& rotation);
    glm::vec3 GetLocalScale() const;
    void SetLocalScale(const glm::vec3& scale);

    virtual glm::mat4 GetTransformationMatrix();
    virtual glm::mat4 GetInverseTransformationMatrix();

    // Did anything in transformation change in this frame
    bool DidTransformationChange() const;
    void SetTransformationChanged(bool changed);

    // Is global position valid
    bool IsPositionDirty() const;
    void SetPositionDirty(bool dirty);

    // Is global rotation valid
    bool IsRotationDirty() const;
    void SetRotationDirty(bool dirty);

    // Is global scale valid
    bool IsScaleDirty() const;
    void SetScaleDirty(bool dirty);

    // Is transformation matrix valid
    bool IsMatrixDirty() const;
    void SetMatrixDirty(bool dirty);

    // Is inverse transformation matrix valid
    bool IsInverseMatrixDirty() const;
    void SetInverseMatrixDirty(bool dirty);

protected:
    glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _scale = { 1.0f, 1.0f, 1.0f };
    glm::vec3 _localPosition = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _localRotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _localScale = { 1.0f, 1.0f, 1.0f };

    static void SetTransformationDirtyRecursively(EntitiesRegistry* registry, EntityID entityID);

    bool transformationChanged = false;
    bool dirtyPosition = false;
    bool dirtyRotation = false;
    bool dirtyScale = false;
    bool dirtyMatrix = true;
    bool dirtyInverseMatrix = true;

private:
    glm::mat4 _transformationMatrix = glm::mat4(1.0f);
    glm::mat4 _inverseTransformationMatrix = glm::mat4(1.0f);
};
