#include "Transformation.h"
#include "SceneHelper.h"
#include "Steel/Core/Log.h"
#include "Steel/UI/RectTransformation.h"
#include "Steel/Serialization/AttributesRegistration.h"

void Transformation::RegisterType()
{
    REGISTER_COMPONENT(Transformation);
    REGISTER_ATTRIBUTE(Transformation, "localPosition", GetLocalPosition, SetLocalPosition, glm::vec3, AttributeFlags::Public, "Position");
    REGISTER_ATTRIBUTE(Transformation, "localRotation", GetLocalRotation, SetLocalRotation, glm::vec3, AttributeFlags::Public, "Rotation");
    REGISTER_ATTRIBUTE(Transformation, "localScale", GetLocalScale, SetLocalScale, glm::vec3, AttributeFlags::Public, "Scale");
}

bool Transformation::Validate(EntitiesRegistry* entitiesRegistry)
{
    if (entitiesRegistry->HasComponent<RectTransformation>(Owner))
    {
        Log::LogError("Adding Transformation to object with RectTransformation will lead to undefined behaviour.");
        return false;
    }

    return Component::Validate(entitiesRegistry);
}

void Transformation::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    SetTransformationChanged(true);
}

const glm::vec3& Transformation::GetPosition()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);
    if (node.GetParentNode() == NULL_ENTITY)
    {
        _position = _localPosition;
    }
    else
    {
        auto& parentTransformation = registry->GetComponent<Transformation>(node.GetParentNode());
        // Transform our local position to parent
        _position = parentTransformation.GetTransformationMatrix() * glm::vec4(_localPosition, 1.0f);
    }

    return _position;
}

void Transformation::SetPosition(const glm::vec3& position)
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);

    _position = position;
    if (node.GetParentNode() == NULL_ENTITY)
    {
        _localPosition = position;
    }
    else
    {
        auto& parentTransform = registry->GetComponent<Transformation>(node.GetParentNode());
        // Transform global position to parent's coordinate system (inverse)
        _localPosition = parentTransform.GetInverseTransformationMatrix() * glm::vec4(_position, 1.0f);
    }

    SetTransformationChanged(true);
}

const glm::vec3& Transformation::GetRotation()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);
    if (node.GetParentNode() == NULL_ENTITY)
    {
        _rotation = _localRotation;
    }
    else
    {
        auto& parentTransformation = registry->GetComponent<Transformation>(node.GetParentNode());
        // For rotation, it should be enough to add parent's rotation to out local
        _rotation = parentTransformation.GetRotation() + _localRotation;
    }

    return _rotation;
}

void Transformation::SetRotation(const glm::vec3& rotation)
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);

    _rotation = rotation;
    if (node.GetParentNode() == NULL_ENTITY)
    {
        _localRotation = _rotation;
    }
    else
    {
        auto& parentTransform = registry->GetComponent<Transformation>(node.GetParentNode());
        // For rotation, it should be enough to subtract parent's rotation from target global
        _localRotation = _rotation - parentTransform.GetRotation();
    }

    SetTransformationChanged(true);
}

const glm::vec3& Transformation::GetScale()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);
    if (node.GetParentNode() == NULL_ENTITY)
    {
        _scale = _localScale;
    }
    else
    {
        auto& parentTransformation = registry->GetComponent<Transformation>(node.GetParentNode());
        // For scale, it should be enough to multiply parent's scale to out local
        _scale = parentTransformation.GetScale() * _localScale;
    }

    return _scale;
}

void Transformation::SetScale(const glm::vec3& scale)
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);

    _scale = scale;
    if (node.GetParentNode() == NULL_ENTITY)
    {
        _localScale = _scale;
    }
    else
    {
        auto& parentTransform = registry->GetComponent<Transformation>(node.GetParentNode());
        // For scale it should be enough to divide target global scale by parent's
        _localScale = _scale / parentTransform.GetScale();
    }

    SetTransformationChanged(true);
}

const glm::vec3& Transformation::GetLocalPosition() const
{
    return _localPosition;
}

void Transformation::SetLocalPosition(const glm::vec3& position)
{
    if (_localPosition == position)
        return;

    _localPosition = position;

    SetTransformationChanged(true);
}

const glm::vec3& Transformation::GetLocalRotation() const
{
    return _localRotation;
}

void Transformation::SetLocalRotation(const glm::vec3& rotation)
{
    if (_localRotation == rotation)
        return;

    _localRotation = rotation;

    SetTransformationChanged(true);
}

const glm::vec3& Transformation::GetLocalScale() const
{
    return _localScale;
}

void Transformation::SetLocalScale(const glm::vec3& scale)
{
    if (_localScale == scale)
        return;

    _localScale = scale;

    SetTransformationChanged(true);
}

const glm::mat4& Transformation::GetTransformationMatrixCached()
{
    return _transformationMatrix;
}

void Transformation::UpdateTransformation(ComponentAccessor<Transformation>& transformationsAccessor, HierarchyNode& hierarchyNode)
{
    if (hierarchyNode.GetParentNode() == NULL_ENTITY)
    {
        if (!DidTransformationChange())
            return;

        _transformationMatrix =
                glm::translate(glm::mat4(1.0f), _localPosition)
                * glm::toMat4(glm::quat(_localRotation))
                * glm::scale(glm::mat4(1.0f), _localScale);
        _position = _localPosition;
    }
    else
    {
        auto& parentTransformation = transformationsAccessor.Get(hierarchyNode.GetParentNode());

        if (!DidTransformationChange() && !parentTransformation.DidTransformationChange())
            return;
        else
            SetTransformationChanged(true);

        _transformationMatrix =
                glm::translate(glm::mat4(1.0f), _localPosition)
                * glm::toMat4(glm::quat(_localRotation))
                * glm::scale(glm::mat4(1.0f), _localScale);

        auto& parentMatrix = parentTransformation.GetTransformationMatrixCached();
        _transformationMatrix = parentMatrix * _transformationMatrix;
        _position = parentMatrix * glm::vec4(_localPosition, 1.0f);
    }
}

const glm::mat4& Transformation::GetTransformationMatrix()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);

    _transformationMatrix =
            glm::translate(glm::mat4(1.0f), _localPosition)
            * glm::toMat4(glm::quat(_localRotation))
            * glm::scale(glm::mat4(1.0f), _localScale);

    if (node.GetParentNode() != NULL_ENTITY)
    {
        auto& parentTransformation = registry->GetComponent<Transformation>(node.GetParentNode());
        _transformationMatrix = parentTransformation.GetTransformationMatrix() * _transformationMatrix;
    }

    return _transformationMatrix;
}

const glm::mat4& Transformation::GetInverseTransformationMatrix()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);

    _transformationMatrix =
            glm::translate(glm::mat4(1.0f), _localPosition)
            * glm::toMat4(glm::quat(_localRotation))
            * glm::scale(glm::mat4(1.0f), _localScale);

    if (node.GetParentNode() != NULL_ENTITY)
    {
        auto& parentTransformation = registry->GetComponent<Transformation>(node.GetParentNode());
        _transformationMatrix = parentTransformation.GetTransformationMatrix() * _transformationMatrix;
    }

    _inverseTransformationMatrix = glm::inverse(_transformationMatrix);

    return _inverseTransformationMatrix;
}

float Transformation::GetGlobalSortingOrderCached() const
{
    return _position.z;
}

bool Transformation::DidTransformationChange() const
{
    return transformationChanged;
}

void Transformation::SetTransformationChanged(bool changed)
{
    transformationChanged = changed;
}

void Transformation::RefreshTransformation()
{
    transformationChanged = false;
}
