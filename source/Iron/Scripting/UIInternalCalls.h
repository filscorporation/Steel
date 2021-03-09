#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include "../Core/ResourceID.h"
#include "../EntityComponentSystem/Entity.h"

namespace UIInternalCalls
{
    // Rect transformation
    void RectTransformation_GetAnchorMin(EntityID entityID, glm::vec2* anchor);
    void RectTransformation_SetAnchorMin(EntityID entityID, glm::vec2* anchor);
    void RectTransformation_GetAnchorMax(EntityID entityID, glm::vec2* anchor);
    void RectTransformation_SetAnchorMax(EntityID entityID, glm::vec2* anchor);
    glm::vec3 RectTransformation_GetAnchoredPosition(EntityID entityID);
    void RectTransformation_SetAnchoredPosition(EntityID entityID, glm::vec3 position);
    void RectTransformation_GetOffsetMin(EntityID entityID, glm::vec2* offset);
    void RectTransformation_SetOffsetMin(EntityID entityID, glm::vec2* offset);
    void RectTransformation_GetOffsetMax(EntityID entityID, glm::vec2* offset);
    void RectTransformation_SetOffsetMax(EntityID entityID, glm::vec2* offset);
    void RectTransformation_GetPivot(EntityID entityID, glm::vec2* pivot);
    void RectTransformation_SetPivot(EntityID entityID, glm::vec2* pivot);
    void RectTransformation_GetSize(EntityID entityID, glm::vec2* size);
    void RectTransformation_SetSize(EntityID entityID, glm::vec2* size);

    // UI image
    ResourceID UIImage_GetSprite(EntityID entityID);
    void UIImage_SetSprite(EntityID entityID, ResourceID spriteID);
};
