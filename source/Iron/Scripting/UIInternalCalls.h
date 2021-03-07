#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace UIInternalCalls
{
    // Rect transformation
    void RectTransformation_GetAnchorMin(int64_t entityID, glm::vec2* anchor);
    void RectTransformation_SetAnchorMin(int64_t entityID, glm::vec2* anchor);
    void RectTransformation_GetAnchorMax(int64_t entityID, glm::vec2* anchor);
    void RectTransformation_SetAnchorMax(int64_t entityID, glm::vec2* anchor);
    glm::vec3 RectTransformation_GetAnchoredPosition(int64_t entityID);
    void RectTransformation_SetAnchoredPosition(int64_t entityID, glm::vec3 position);
    void RectTransformation_GetOffsetMin(int64_t entityID, glm::vec2* offset);
    void RectTransformation_SetOffsetMin(int64_t entityID, glm::vec2* offset);
    void RectTransformation_GetOffsetMax(int64_t entityID, glm::vec2* offset);
    void RectTransformation_SetOffsetMax(int64_t entityID, glm::vec2* offset);
    void RectTransformation_GetPivot(int64_t entityID, glm::vec2* pivot);
    void RectTransformation_SetPivot(int64_t entityID, glm::vec2* pivot);
    void RectTransformation_GetSize(int64_t entityID, glm::vec2* size);
    void RectTransformation_SetSize(int64_t entityID, glm::vec2* size);

    // UI image
    uint64_t UIImage_GetSprite(int64_t entityID);
    void UIImage_SetSprite(int64_t entityID, int64_t spriteID);
};
