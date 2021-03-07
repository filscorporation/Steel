#include "RectTransformation.h"
#include "../Rendering/Screen.h"
#include "../Core/Log.h"

// TODO: use in sets
#define TRANSFORM_EPS 0.000001f

glm::mat4 RectTransformation::GetTransformationMatrix()
{
    if (IsTransformationDirty() || Screen::IsScreenSizeDirty())
    {
        SetTransformationDirty(false);

        // TODO: apply parent transformations, for now parent is screen
        glm::vec2 parentSize = glm::vec2(Screen::GetWidth(), Screen::GetHeight());

        glm::vec3 realPosition;
        glm::vec3 realScale;
        for (int i = 0; i < 2; ++i)
        {
            if (std::abs(_anchorMin[i] - _anchorMax[i]) < TRANSFORM_EPS)
            {
                realScale[i] = _scale[i] * _size[i];
                realPosition[i] = parentSize[i] * _anchorMin[i] + _anchoredPosition[i] + (_pivot[i] - 0.5f) * realScale[i];
            }
            else
            {
                float xMin = parentSize[i] * _anchorMin[i] + _offsetMin[i];
                float xMax = parentSize[i] * _anchorMax[i] - _offsetMax[i];
                realScale[i] = xMax - xMin;
                realPosition[i] = xMin + 0.5f * realScale[i];
            }
        }
        realPosition.z = _anchoredPosition.z;
        realScale.z = 1.0f;

        glm::mat4 rotationMatrix = glm::toMat4(glm::quat(_rotation)); // TODO: rotate around pivot
        _transformationMatrix = glm::translate(glm::mat4(1.0f), realPosition)
                                * rotationMatrix
                                * glm::scale(glm::mat4(1.0f), realScale);
    }

    return _transformationMatrix;
}

glm::vec3 RectTransformation::GetPosition() const
{
    return GetAnchoredPosition();
}

void RectTransformation::SetPosition(const glm::vec3 &position)
{
    SetAnchoredPosition(position);
}

glm::vec2 RectTransformation::GetAnchorMin() const
{
    return _anchorMin;
}

void RectTransformation::SetAnchorMin(const glm::vec2& anchor)
{
    SetTransformationDirty(true);

    _anchorMin = anchor;
    _anchorMax.x = std::max(_anchorMin.x, _anchorMax.x);
    _anchorMax.y = std::max(_anchorMin.y, _anchorMax.y);
}

glm::vec2 RectTransformation::GetAnchorMax() const
{
    return _anchorMax;
}

void RectTransformation::SetAnchorMax(const glm::vec2& anchor)
{
    SetTransformationDirty(true);

    _anchorMax = anchor;
    _anchorMin.x = std::min(_anchorMin.x, _anchorMax.x);
    _anchorMin.y = std::min(_anchorMin.y, _anchorMax.y);
}

glm::vec3 RectTransformation::GetAnchoredPosition() const
{
    return _anchoredPosition;
}

void RectTransformation::SetAnchoredPosition(const glm::vec3& position)
{
    SetTransformationDirty(true);

    _anchoredPosition = position;
}

glm::vec2 RectTransformation::GetOffsetMin() const
{
    return _offsetMin;
}

void RectTransformation::SetOffsetMin(const glm::vec2& offset)
{
    SetTransformationDirty(true);

    _offsetMin = offset;
}

glm::vec2 RectTransformation::GetOffsetMax() const
{
    return _offsetMax;
}

void RectTransformation::SetOffsetMax(const glm::vec2& offset)
{
    SetTransformationDirty(true);

    _offsetMax = offset;
}

glm::vec2 RectTransformation::GetSize() const
{
    return _size;
}

void RectTransformation::SetSize(const glm::vec2& size)
{
    SetTransformationDirty(true);

    _size = glm::vec2(std::max(size.x, 0.0f), std::max(size.y, 0.0f));
}

glm::vec2 RectTransformation::GetPivot() const
{
    return _pivot;
}

void RectTransformation::RectTransformation::SetPivot(const glm::vec2& pivot)
{
    SetTransformationDirty(true);

    _pivot = pivot;
}

void RectTransformation::UpdatePhysicsTransformation()
{
    // UI can't have physics
}
