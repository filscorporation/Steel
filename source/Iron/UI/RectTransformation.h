#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "../EntityComponentSystem/Component.h"
#include "../EntityComponentSystem/EntitiesRegistry.h"
#include "../Scene/HierarchyNode.h"

class RectTransformation : public Component
{
public:
    explicit RectTransformation(EntityID ownerEntityID) : Component(ownerEntityID) { }

    glm::vec2 GetAnchorMin() const;
    void SetAnchorMin(const glm::vec2& anchor);
    glm::vec2 GetAnchorMax() const;
    void SetAnchorMax(const glm::vec2& anchor);
    glm::vec2 GetAnchoredPosition() const;
    void SetAnchoredPosition(const glm::vec2& position);
    glm::vec2 GetOffsetMin() const;
    void SetOffsetMin(const glm::vec2& offset);
    glm::vec2 GetOffsetMax() const;
    void SetOffsetMax(const glm::vec2& offset);
    glm::vec2 GetSize() const;
    void SetSize(const glm::vec2& size);
    glm::vec2 GetPivot() const;
    void SetPivot(const glm::vec2& pivot);
    glm::vec3 GetRotation();
    void SetRotation(const glm::vec3& rotation);
    glm::vec3 GetLocalRotation() const;
    void SetLocalRotation(const glm::vec3& rotation);

    virtual const glm::mat4& GetTransformationMatrixCached();
    virtual void UpdateTransformation(ComponentAccessor<RectTransformation>& rtAccessor, HierarchyNode& hierarchyNode);
    float GetSortingOrder() const;
    void SetSortingOrder(float sortingOrder);
    float GetGlobalSortingOrder();
    float GetGlobalSortingOrderCached() const;

    // Did anything in transformation change in this frame
    bool DidTransformationChange() const;
    void SetTransformationChanged(bool changed);

private:
    glm::mat4 _transformationMatrix = glm::mat4(1.0f);

    glm::vec2 _anchorMin = { 0.0f, 0.0f };
    glm::vec2 _anchorMax = { 0.0f, 0.0f };
    glm::vec2 _anchoredPosition = { 0.0f, 0.0f };
    float _sortingOrder = 0.0f;
    float _globalSortingOrder = 0.0f;
    glm::vec2 _offsetMin = { 0.0f, 0.0f };
    glm::vec2 _offsetMax = { 0.0f, 0.0f };
    glm::vec2 _size = { 0.0f, 0.0f };
    glm::vec2 _pivot = { 0.5f, 0.5f };
    glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 _localRotation = { 0.0f, 0.0f, 0.0f };

    bool transformationChanged = true;
};
