#include "../Core/Application.h"
#include "Transformation.h"
#include "HierarchyNode.h"
#include "Hierarchy.h"

#define TRANSFORM_EPS 0.000001f

glm::vec3 Transformation::GetPosition()
{
    if (IsPositionDirty())
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        auto& node = registry->GetComponent<HierarchyNode>(Owner);
        if (node.ParentNode == NULL_ENTITY)
        {
            _position = _localPosition;
        }
        else
        {
            auto& parentTransformation = registry->GetComponent<Transformation>(node.ParentNode);
            // Transform our local position to parent
            _position = parentTransformation.GetTransformationMatrix() * glm::vec4(_localPosition, 1.0f);
        }

        SetPositionDirty(false);
    }

    return _position;
}

void Transformation::SetPosition(const glm::vec3& position)
{
    if (!IsPositionDirty()
        && std::abs(_position.x - position.x) < TRANSFORM_EPS
        && std::abs(_position.y - position.y) < TRANSFORM_EPS
        && std::abs(_position.z - position.z) < TRANSFORM_EPS)
        return;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);

    _position = position;
    if (node.ParentNode == NULL_ENTITY)
    {
        _localPosition = position;
    }
    else
    {
        auto& parentTransform = registry->GetComponent<Transformation>(node.ParentNode);
        // Transform global position to parent's coordinate system (inverse)
        _localPosition = parentTransform.GetInverseTransformationMatrix() * glm::vec4(_position, 1.0f);
    }

    SetTransformationChanged(true);
    SetMatrixDirty(true);
    SetInverseMatrixDirty(true);
    ForeachChildren(registry, node, SetTransformationDirtyRecursively);
}

glm::vec3 Transformation::GetRotation()
{
    if (IsRotationDirty())
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        auto& node = registry->GetComponent<HierarchyNode>(Owner);
        if (node.ParentNode == NULL_ENTITY)
        {
            _rotation = _localRotation;
        }
        else
        {
            auto& parentTransformation = registry->GetComponent<Transformation>(node.ParentNode);
            // For rotation it should be enough to add parent's rotation to out local
            _rotation = parentTransformation.GetRotation() + _localRotation;
        }

        SetRotationDirty(false);
    }

    return _rotation;
}

void Transformation::SetRotation(const glm::vec3& rotation)
{
    if (!IsRotationDirty()
        && std::abs(_rotation.x - rotation.x) < TRANSFORM_EPS
        && std::abs(_rotation.y - rotation.y) < TRANSFORM_EPS
        && std::abs(_rotation.z - rotation.z) < TRANSFORM_EPS)
        return;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);

    _rotation = rotation;
    if (node.ParentNode == NULL_ENTITY)
    {
        _localRotation = _rotation;
    }
    else
    {
        auto& parentTransform = registry->GetComponent<Transformation>(node.ParentNode);
        // For rotation it should be enough to subtract parent's rotation from target global
        _localRotation = _rotation - parentTransform.GetRotation();
    }

    SetTransformationChanged(true);
    SetMatrixDirty(true);
    SetInverseMatrixDirty(true);
    ForeachChildren(registry, node, SetTransformationDirtyRecursively);
}

glm::vec3 Transformation::GetScale()
{
    if (IsScaleDirty())
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        auto& node = registry->GetComponent<HierarchyNode>(Owner);
        if (node.ParentNode == NULL_ENTITY)
        {
            _scale = _localScale;
        }
        else
        {
            auto& parentTransformation = registry->GetComponent<Transformation>(node.ParentNode);
            // For scale it should be enough to multiply parent's scale to out local
            _scale = parentTransformation.GetScale() * _localScale;
        }

        SetScaleDirty(false);
    }

    return _scale;
}

void Transformation::SetScale(const glm::vec3& scale)
{
    if (!IsScaleDirty()
        && std::abs(_scale.x - scale.x) < TRANSFORM_EPS
        && std::abs(_scale.y - scale.y) < TRANSFORM_EPS
        && std::abs(_scale.z - scale.z) < TRANSFORM_EPS)
        return;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);

    _scale = scale;
    if (node.ParentNode == NULL_ENTITY)
    {
        _localScale = _scale;
    }
    else
    {
        auto& parentTransform = registry->GetComponent<Transformation>(node.ParentNode);
        // For scale it should be enough to divide target global scale by parent's
        _localScale = _scale / parentTransform.GetScale();
    }

    SetTransformationChanged(true);
    SetMatrixDirty(true);
    SetInverseMatrixDirty(true);
    ForeachChildren(registry, node, SetTransformationDirtyRecursively);
}

glm::vec3 Transformation::GetLocalPosition() const
{
    return _localPosition;
}

void Transformation::SetLocalPosition(const glm::vec3& position)
{
    if (   std::abs(_localPosition.x - position.x) < TRANSFORM_EPS
        && std::abs(_localPosition.y - position.y) < TRANSFORM_EPS
        && std::abs(_localPosition.z - position.z) < TRANSFORM_EPS)
        return;

    _localPosition = position;

    SetTransformationChanged(true);
    SetPositionDirty(true);
    SetMatrixDirty(true);
    SetInverseMatrixDirty(true);

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);
    ForeachChildren(registry, node, SetTransformationDirtyRecursively);
}

glm::vec3 Transformation::GetLocalRotation() const
{
    return _localRotation;
}

void Transformation::SetLocalRotation(const glm::vec3& rotation)
{
    if (   std::abs(_localRotation.x - rotation.x) < TRANSFORM_EPS
        && std::abs(_localRotation.y - rotation.y) < TRANSFORM_EPS
        && std::abs(_localRotation.z - rotation.z) < TRANSFORM_EPS)
        return;

    _localRotation = rotation;

    SetTransformationChanged(true);
    SetRotationDirty(true);
    SetMatrixDirty(true);
    SetInverseMatrixDirty(true);

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);
    ForeachChildren(registry, node, SetTransformationDirtyRecursively);
}

glm::vec3 Transformation::GetLocalScale() const
{
    return _localScale;
}

void Transformation::SetLocalScale(const glm::vec3& scale)
{
    if (   std::abs(_localScale.x - scale.x) < TRANSFORM_EPS
           && std::abs(_localScale.y - scale.y) < TRANSFORM_EPS
           && std::abs(_localScale.z - scale.z) < TRANSFORM_EPS)
        return;

    _localScale = scale;

    SetTransformationChanged(true);
    SetScaleDirty(true);
    SetMatrixDirty(true);
    SetInverseMatrixDirty(true);

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);
    ForeachChildren(registry, node, SetTransformationDirtyRecursively);
}

glm::mat4 Transformation::GetTransformationMatrix()
{
    if (IsMatrixDirty())
    {
        SetMatrixDirty(false);

        _transformationMatrix =
                glm::translate(glm::mat4(1.0f), GetPosition())
               * glm::toMat4(glm::quat(GetRotation()))
               * glm::scale(glm::mat4(1.0f), GetScale());
    }

    return _transformationMatrix;
}

glm::mat4 Transformation::GetInverseTransformationMatrix()
{
    if (IsInverseMatrixDirty())
    {
        SetInverseMatrixDirty(false);

        _inverseTransformationMatrix = glm::inverse(GetTransformationMatrix());
    }

    return _inverseTransformationMatrix;
}

void Transformation::SetTransformationDirtyRecursively(EntitiesRegistry* registry, EntityID entityID)
{
    auto& transform = registry->GetComponent<Transformation>(entityID);
    transform.SetTransformationChanged(true);
    transform.SetPositionDirty(true);
    transform.SetRotationDirty(true);
    transform.SetScaleDirty(true);
    transform.SetMatrixDirty(true);
    transform.SetInverseMatrixDirty(true);
}

bool Transformation::DidTransformationChange() const
{
    return transformationChanged;
}

void Transformation::SetTransformationChanged(bool changed)
{
    transformationChanged = changed;
}

bool Transformation::IsPositionDirty() const
{
    return dirtyPosition;
}

void Transformation::SetPositionDirty(bool dirty)
{
    dirtyPosition = dirty;
}

bool Transformation::IsRotationDirty() const
{
    return dirtyRotation;
}

void Transformation::SetRotationDirty(bool dirty)
{
    dirtyRotation = dirty;
}

bool Transformation::IsScaleDirty() const
{
    return dirtyScale;
}

void Transformation::SetScaleDirty(bool dirty)
{
    dirtyScale = dirty;
}

bool Transformation::IsMatrixDirty() const
{
    return dirtyMatrix;
}

void Transformation::SetMatrixDirty(bool dirty)
{
    dirtyMatrix = dirty;
}

bool Transformation::IsInverseMatrixDirty() const
{
    return dirtyInverseMatrix;
}

void Transformation::SetInverseMatrixDirty(bool dirty)
{
    dirtyInverseMatrix = dirty;
}
