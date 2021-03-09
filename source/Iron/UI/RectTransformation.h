#pragma once

#include "../Scene/Transformation.h"

class RectTransformation : public Transformation
{
public:
    explicit RectTransformation(EntityID ownerEntityID) : Transformation(ownerEntityID) { }

    glm::mat4 GetTransformationMatrix() override;

    glm::vec3 GetPosition() const override;
    void SetPosition(const glm::vec3& position) override;

    glm::vec2 GetAnchorMin() const;
    void SetAnchorMin(const glm::vec2& anchor);
    glm::vec2 GetAnchorMax() const;
    void SetAnchorMax(const glm::vec2& anchor);
    glm::vec3 GetAnchoredPosition() const;
    void SetAnchoredPosition(const glm::vec3& position);
    glm::vec2 GetOffsetMin() const;
    void SetOffsetMin(const glm::vec2& offset);
    glm::vec2 GetOffsetMax() const;
    void SetOffsetMax(const glm::vec2& offset);
    glm::vec2 GetSize() const;
    void SetSize(const glm::vec2& size);
    glm::vec2 GetPivot() const;
    void SetPivot(const glm::vec2& pivot);

    void UpdatePhysicsTransformation() override;

private:
    glm::mat4 _transformationMatrix;

    glm::vec2 _anchorMin = { 0.0f, 0.0f };
    glm::vec2 _anchorMax = { 0.0f, 0.0f };
    glm::vec3 _anchoredPosition = { 0.0f, 0.0f, 0.0f };
    glm::vec2 _offsetMin = { 0.0f, 0.0f };
    glm::vec2 _offsetMax = { 0.0f, 0.0f };
    glm::vec2 _size = { 0.0f, 0.0f };
    glm::vec2 _pivot = { 0.5f, 0.5f };
};
