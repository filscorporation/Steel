#include "RectTransformation.h"
#include "../Rendering/Screen.h"
#include "../Core/Application.h"
#include "../Core/Log.h"
#include "../Scene/SceneHelper.h"
#include "../Scene/Transformation.h"

#define TRANSFORM_EPS 0.000001f

RectTransformation::RectTransformation(EntityID ownerEntityID) : Component(ownerEntityID)
{
    if (HasComponentS<Transformation>(ownerEntityID))
    {
        Log::LogError("Adding Transformation to objects with RectTransformation will lead to undefined behaviour.");
    }
}

glm::vec2 RectTransformation::GetAnchorMin() const
{
    return _anchorMin;
}

void RectTransformation::SetAnchorMin(const glm::vec2& anchor)
{
    if (   std::abs(_anchorMin.x - anchor.x) < TRANSFORM_EPS
        && std::abs(_anchorMin.y - anchor.y) < TRANSFORM_EPS)
        return;

    _anchorMin = anchor;
    _anchorMax.x = std::max(_anchorMin.x, _anchorMax.x);
    _anchorMax.y = std::max(_anchorMin.y, _anchorMax.y);

    SetTransformationChanged(true);
}

glm::vec2 RectTransformation::GetAnchorMax() const
{
    return _anchorMax;
}

void RectTransformation::SetAnchorMax(const glm::vec2& anchor)
{
    if (   std::abs(_anchorMax.x - anchor.x) < TRANSFORM_EPS
        && std::abs(_anchorMax.y - anchor.y) < TRANSFORM_EPS)
        return;

    _anchorMax = anchor;
    _anchorMin.x = std::min(_anchorMin.x, _anchorMax.x);
    _anchorMin.y = std::min(_anchorMin.y, _anchorMax.y);

    SetTransformationChanged(true);
}

glm::vec2 RectTransformation::GetAnchoredPosition() const
{
    return _anchoredPosition;
}

void RectTransformation::SetAnchoredPosition(const glm::vec2& position)
{
    if (   std::abs(_anchoredPosition.x - position.x) < TRANSFORM_EPS
        && std::abs(_anchoredPosition.y - position.y) < TRANSFORM_EPS)
        return;

    _anchoredPosition = position;

    SetTransformationChanged(true);
}

glm::vec2 RectTransformation::GetOffsetMin() const
{
    return _offsetMin;
}

void RectTransformation::SetOffsetMin(const glm::vec2& offset)
{
    if (   std::abs(_offsetMin.x - offset.x) < TRANSFORM_EPS
        && std::abs(_offsetMin.y - offset.y) < TRANSFORM_EPS)
        return;

    _offsetMin = offset;

    SetTransformationChanged(true);
}

glm::vec2 RectTransformation::GetOffsetMax() const
{
    return _offsetMax;
}

void RectTransformation::SetOffsetMax(const glm::vec2& offset)
{
    if (   std::abs(_offsetMax.x - offset.x) < TRANSFORM_EPS
        && std::abs(_offsetMax.y - offset.y) < TRANSFORM_EPS)
        return;

    _offsetMax = offset;

    SetTransformationChanged(true);
}

glm::vec2 RectTransformation::GetSize() const
{
    return _size;
}

glm::vec2 RectTransformation::GetRealSizeCached() const
{
    return _realSize;
}

void RectTransformation::SetSize(const glm::vec2& size)
{
    if (   std::abs(_size.x - size.x) < TRANSFORM_EPS
        && std::abs(_size.y - size.y) < TRANSFORM_EPS)
        return;

    _size = glm::vec2(std::max(size.x, 0.0f), std::max(size.y, 0.0f));

    SetTransformationChanged(true);
    sizeChanged = true;
}

glm::vec2 RectTransformation::GetPivot() const
{
    return _pivot;
}

void RectTransformation::RectTransformation::SetPivot(const glm::vec2& pivot)
{
    if (   std::abs(_pivot.x - pivot.x) < TRANSFORM_EPS
        && std::abs(_pivot.y - pivot.y) < TRANSFORM_EPS)
        return;

    _pivot = pivot;

    SetTransformationChanged(true);
}

glm::vec3 RectTransformation::GetRotation()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);
    if (node.ParentNode == NULL_ENTITY)
    {
        _rotation = _localRotation;
    }
    else
    {
        auto& parentRT = registry->GetComponent<RectTransformation>(node.ParentNode);
        // For rotation it should be enough to add parent's rotation to out local
        _rotation = parentRT.GetRotation() + _localRotation;
    }

    return _rotation;
}

void RectTransformation::SetRotation(const glm::vec3& rotation)
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);

    _rotation = rotation;
    if (node.ParentNode == NULL_ENTITY)
    {
        _localRotation = _rotation;
    }
    else
    {
        auto& parentRT = registry->GetComponent<RectTransformation>(node.ParentNode);
        // For rotation it should be enough to subtract parent's rotation from target global
        _localRotation = _rotation - parentRT.GetRotation();
    }

    SetTransformationChanged(true);
}

glm::vec3 RectTransformation::GetLocalRotation() const
{
    return _localRotation;
}

void RectTransformation::SetLocalRotation(const glm::vec3& rotation)
{
    if (   std::abs(_localRotation.x - rotation.x) < TRANSFORM_EPS
        && std::abs(_localRotation.y - rotation.y) < TRANSFORM_EPS
        && std::abs(_localRotation.z - rotation.z) < TRANSFORM_EPS)
        return;

    _localRotation = rotation;

    SetTransformationChanged(true);
}

bool RectTransformation::Contains(const glm::vec2& point) const
{
    // TODO: does not use rotation
    return point.x > _realPosition.x - _realSize.x * 0.5f
        && point.x < _realPosition.x + _realSize.x * 0.5f
        && point.y > _realPosition.y - _realSize.y * 0.5f
        && point.y < _realPosition.y + _realSize.y * 0.5f;
}

const glm::mat4& RectTransformation::GetTransformationMatrixCached()
{
    return _transformationMatrix;
}

void RectTransformation::UpdateTransformation(ComponentAccessor<RectTransformation>& rtAccessor, HierarchyNode& hierarchyNode)
{
    glm::vec2 parentSize;
    glm::vec2 parentPosition;

    if (hierarchyNode.ParentNode == NULL_ENTITY)
    {
        if (!DidTransformationChange() && !Screen::IsScreenSizeDirty())
            return;
        else
        {
            SetTransformationChanged(true);
            if (Screen::IsScreenSizeDirty())
                sizeChanged = true;
        }

        parentSize = glm::vec2(Screen::GetWidth(), Screen::GetHeight());
        parentPosition = parentSize * 0.5f;
        _globalSortingOrder = _sortingOrder;
    }
    else
    {
        auto& parentRT = rtAccessor.Get(hierarchyNode.ParentNode);

        if (!DidTransformationChange() && !parentRT.DidTransformationChange() && !Screen::IsScreenSizeDirty())
            return;
        else
        {
            SetTransformationChanged(true);
            if (Screen::IsScreenSizeDirty() || parentRT.DidSizeChange())
                sizeChanged = true;
        }

        parentSize = parentRT.GetRealSizeCached();
        parentPosition = parentRT.GetRealPositionCached();
        _globalSortingOrder = parentRT.GetGlobalSortingOrderCached() + _sortingOrder;
    }

    for (int i = 0; i < 2; ++i)
    {
        if (std::abs(_anchorMin[i] - _anchorMax[i]) < TRANSFORM_EPS)
        {
            _realPosition[i] = parentPosition[i] + parentSize[i] * (_anchorMin[i] - 0.5f) + _size[i] * (0.5f - _pivot[i]) + _anchoredPosition[i];
            _realSize[i] = _size[i];
        }
        else
        {
            float min = parentPosition[i] + parentSize[i] * (_anchorMin[i] - 0.5f) + _offsetMin[i];
            float max = parentPosition[i] + parentSize[i] * (_anchorMax[i] - 0.5f) - _offsetMax[i];
            _realSize[i] = max - min;
            _realPosition[i] = min + 0.5f * _realSize[i];
        }
    }

    _transformationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(_realPosition, -_globalSortingOrder))
                            * glm::toMat4(glm::quat(_rotation))
                            * glm::scale(glm::mat4(1.0f), glm::vec3(_realSize, 1.0f));
}

float RectTransformation::GetSortingOrder() const
{
    return _sortingOrder;
}

void RectTransformation::SetSortingOrder(float sortingOrder)
{
    if (std::abs(_sortingOrder - sortingOrder) < TRANSFORM_EPS)
        return;

    _sortingOrder = sortingOrder;

    SetTransformationChanged(true);
}

float RectTransformation::GetGlobalSortingOrder()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);
    if (node.ParentNode == NULL_ENTITY)
    {
        _globalSortingOrder = _sortingOrder;
    }
    else
    {
        auto& parentRT = registry->GetComponent<RectTransformation>(node.ParentNode);
        _globalSortingOrder = parentRT.GetGlobalSortingOrder() + _sortingOrder;
    }

    return _globalSortingOrder;
}

float RectTransformation::GetGlobalSortingOrderCached() const
{
    return _globalSortingOrder;
}

const glm::vec2& RectTransformation::GetRealPositionCached()
{
    return _realPosition;
}

bool RectTransformation::DidTransformationChange() const
{
    return transformationChanged;
}

void RectTransformation::SetTransformationChanged(bool changed)
{
    transformationChanged = changed;
}

bool RectTransformation::DidSizeChange() const
{
    return sizeChanged;
}

void RectTransformation::RefreshTransformation()
{
    transformationChanged = false;
    sizeChanged = false;
}
