#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "UILayer.h"
#include "Steel/EntityComponentSystem/Component.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"
#include "Steel/Scene/HierarchyNode.h"

#define PIXEL_CORRECTION true

class RectTransformation : public Component
{
    DEFINE_TYPE(RectTransformation)

public:
    explicit RectTransformation(EntityID ownerEntityID) : Component(ownerEntityID) { };

    bool Validate(EntitiesRegistry* entitiesRegistry) override;
    void OnEnabled(EntitiesRegistry* entitiesRegistry) override;

    const glm::vec2& GetAnchorMin() const;
    void SetAnchorMin(const glm::vec2& anchor);
    const glm::vec2& GetAnchorMax() const;
    void SetAnchorMax(const glm::vec2& anchor);
    const glm::vec2& GetAnchoredPosition() const;
    void SetAnchoredPosition(const glm::vec2& position);
    const glm::vec2& GetOffsetMin() const;
    void SetOffsetMin(const glm::vec2& offset);
    const glm::vec2& GetOffsetMax() const;
    void SetOffsetMax(const glm::vec2& offset);
    const glm::vec2& GetSize() const;
    void SetSize(const glm::vec2& size);
    const glm::vec2& GetPivot() const;
    void SetPivot(const glm::vec2& pivot);
    const glm::vec3& GetRotation();
    void SetRotation(const glm::vec3& rotation);
    const glm::vec3& GetLocalRotation() const;
    void SetLocalRotation(const glm::vec3& rotation);

    bool Contains(const glm::vec2& point) const;

    const glm::vec2& GetRealSizeCached() const;
    const glm::mat4& GetTransformationMatrixCached();
    void UpdateTransformation(UILayer* layer, ComponentAccessor<RectTransformation>& rtAccessor, HierarchyNode& hierarchyNode);
    const glm::vec2& GetRealPositionCached();
    float GetSortingOrder() const;
    uint32_t GetCurrentHierarchyIndex() const;
    uint32_t GetChildrenThickness() const;
    void IncreaseCurrentThickness(uint32_t thickness);
    bool GetParallelHierarchy() const;
    void SetParallelHierarchy(bool isParallel);

    // Did anything in transformation change in this frame
    bool DidTransformationChange() const;
    void SetTransformationChanged(bool changed);
    bool DidSizeChange() const;
    void RefreshTransformation();

private:
    glm::mat4 _transformationMatrix = glm::mat4(1.0f);

    glm::vec2 _anchorMin = { 0.0f, 0.0f };
    glm::vec2 _anchorMax = { 0.0f, 0.0f };
    glm::vec2 _anchoredPosition = { 0.0f, 0.0f };
    glm::vec2 _offsetMin = { 0.0f, 0.0f };
    glm::vec2 _offsetMax = { 0.0f, 0.0f };
    glm::vec2 _size = { 0.0f, 0.0f };
    glm::vec2 _realSize = { 0.0f, 0.0f };
    glm::vec2 _pivot = { 0.5f, 0.5f };
    glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _localRotation = { 0.0f, 0.0f, 0.0f };
    glm::vec2 _realPosition = { 0.0f, 0.0f };

    bool _parallelHierarchy = false;
    uint32_t _currentThickness = 0;
    uint32_t _hierarchyIndex = 0;
    float _sortingOrder = 0.0f;

    bool transformationChanged = true;
    bool sizeChanged = true;
};
