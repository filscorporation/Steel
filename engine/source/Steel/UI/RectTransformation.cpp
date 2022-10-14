#include "RectTransformation.h"
#include "Steel/Rendering/Screen.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Scene/Transformation.h"

#define TRANSFORM_EPS 0.000001f

void RectTransformation::RegisterType()
{
    REGISTER_COMPONENT(RectTransformation);
    REGISTER_ATTRIBUTE(RectTransformation, "anchorMin", GetAnchorMin, SetAnchorMin, glm::vec2, AttributeFlags::Public, "Anchor min");
    REGISTER_ATTRIBUTE(RectTransformation, "anchorMax", GetAnchorMax, SetAnchorMax, glm::vec2, AttributeFlags::Public, "Anchor max");
    REGISTER_ATTRIBUTE(RectTransformation, "anchoredPosition", GetAnchoredPosition, SetAnchoredPosition, glm::vec2, AttributeFlags::Public, "Anchored position");
    REGISTER_ATTRIBUTE(RectTransformation, "offsetMin", GetOffsetMin, SetOffsetMin, glm::vec2, AttributeFlags::Public, "Offset min");
    REGISTER_ATTRIBUTE(RectTransformation, "offsetMax", GetOffsetMax, SetOffsetMax, glm::vec2, AttributeFlags::Public, "Offset max");
    REGISTER_ATTRIBUTE(RectTransformation, "size", GetSize, SetSize, glm::vec2, AttributeFlags::Public, "Size");
    REGISTER_ATTRIBUTE(RectTransformation, "pivot", GetPivot, SetPivot, glm::vec2, AttributeFlags::Public, "Pivot");
    REGISTER_ATTRIBUTE(RectTransformation, "localRotation", GetLocalRotation, SetLocalRotation, glm::vec3, AttributeFlags::Public, "Rotation");
}

bool RectTransformation::Validate(EntitiesRegistry* entitiesRegistry)
{
    if (entitiesRegistry->HasComponent<Transformation>(Owner))
    {
        Log::LogError("Adding RectTransformation to objects with Transformation will lead to undefined behaviour.");
        return false;
    }

    return Component::Validate(entitiesRegistry);
}

void RectTransformation::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    SetTransformationChanged(true);
}

const glm::vec2& RectTransformation::GetAnchorMin() const
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
    sizeChanged = true;
}

const glm::vec2& RectTransformation::GetAnchorMax() const
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
    sizeChanged = true;
}

const glm::vec2& RectTransformation::GetAnchoredPosition() const
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

const glm::vec2& RectTransformation::GetOffsetMin() const
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
    sizeChanged = true;
}

const glm::vec2& RectTransformation::GetOffsetMax() const
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
    sizeChanged = true;
}

const glm::vec2& RectTransformation::GetSize() const
{
    return _size;
}

const glm::vec2& RectTransformation::GetRealSizeCached() const
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

const glm::vec2& RectTransformation::GetPivot() const
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

const glm::vec3& RectTransformation::GetRotation()
{
    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& node = registry->GetComponent<HierarchyNode>(Owner);
    if (node.GetParentNode() == NULL_ENTITY)
    {
        _rotation = _localRotation;
    }
    else
    {
        auto& parentRT = registry->GetComponent<RectTransformation>(node.GetParentNode());
        // For rotation, it should be enough to add parent's rotation to out local
        _rotation = parentRT.GetRotation() + _localRotation;
    }

    return _rotation;
}

void RectTransformation::SetRotation(const glm::vec3& rotation)
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
        auto& parentRT = registry->GetComponent<RectTransformation>(node.GetParentNode());
        // For rotation, it should be enough to subtract parent's rotation from target global
        _localRotation = _rotation - parentRT.GetRotation();
    }

    SetTransformationChanged(true);
}

const glm::vec3& RectTransformation::GetLocalRotation() const
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

uint32_t RectTransformation::GetCurrentHierarchyIndex() const
{
    return _hierarchyIndex + _currentThickness;
}

uint32_t RectTransformation::GetChildrenThickness() const
{
    return _currentThickness - DEFAULT_THICKNESS;
}

void RectTransformation::IncreaseCurrentThickness(uint32_t thickness)
{
    if (!_parallelHierarchy)
        _currentThickness += thickness;
}

bool RectTransformation::GetParallelHierarchy() const
{
    return _parallelHierarchy;
}

void RectTransformation::SetParallelHierarchy(bool isParallel)
{
    _parallelHierarchy = isParallel;
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

void RectTransformation::UpdateTransformation(UILayer* layer, ComponentAccessor<RectTransformation>& rtAccessor, HierarchyNode& hierarchyNode)
{
    glm::vec2 parentSize;
    glm::vec2 parentPosition;

    if (hierarchyNode.GetParentNode() == NULL_ENTITY)
    {
        if (!DidTransformationChange() && !Screen::IsScreenSizeDirty())
        {
            if (!layer->NeedRebuildSortingOrder())
                return;
        }
        else
        {
            SetTransformationChanged(true);
            if (Screen::IsScreenSizeDirty())
                sizeChanged = true;
        }

        parentSize = glm::vec2(Screen::GetWidth(), Screen::GetHeight());
        parentPosition = parentSize * 0.5f;
        _hierarchyIndex = layer->GetCurrentHierarchyIndex();
        layer->IncreaseCurrentHierarchyIndex(hierarchyNode.GetThickness());
    }
    else
    {
        auto& parentRT = rtAccessor.Get(hierarchyNode.GetParentNode());

        if (!DidTransformationChange() && !parentRT.DidTransformationChange() && !Screen::IsScreenSizeDirty())
        {
            if (!layer->NeedRebuildSortingOrder())
                return;
        }
        else
        {
            SetTransformationChanged(true);
            if (Screen::IsScreenSizeDirty() || parentRT.DidSizeChange())
                sizeChanged = true;
        }

        parentSize = parentRT.GetRealSizeCached();
        parentPosition = parentRT.GetRealPositionCached();
        _hierarchyIndex = parentRT.GetCurrentHierarchyIndex();
        parentRT.IncreaseCurrentThickness(hierarchyNode.GetThickness());
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

    _currentThickness = DEFAULT_THICKNESS;
    if (layer->NeedRebuildSortingOrder())
        // UI elements deeper in hierarchy will get higher hierarchyIndex, but should get rendered earlier (for opaque)
        // For this reason we invert their sorting order (elements renderer ascending by sorting order)
        // But z position is (1 - sortingOrder) because UI camera is pointing to negative z
        _sortingOrder = 1.0f - (float)(_hierarchyIndex + _currentThickness) / (float)layer->GetLayerThickness();

    // Pixel correction is fixing bug caused by rounding error when quad position is in the middle of pixel
    // Shifting position will prevent this bug for rects with uneven size
    const float d = PIXEL_CORRECTION ? 0.01f : 0.0f;
    glm::vec3 pixelCorrection = glm::vec3(d, d, 0.0f);
    _transformationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(_realPosition, 1.0f - _sortingOrder) + pixelCorrection)
                            * glm::toMat4(glm::quat(_rotation))
                            * glm::scale(glm::mat4(1.0f), glm::vec3(_realSize, 1.0f));
}

float RectTransformation::GetSortingOrder() const
{
    return _sortingOrder;
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
