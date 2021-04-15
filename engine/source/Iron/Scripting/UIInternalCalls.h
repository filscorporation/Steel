#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include "ScriptingCore.h"
#include "../EntityComponentSystem/Entity.h"
#include "../Resources/ResourceID.h"
#include "../UI/UIElements/UIText.h"
#include "../UI/UIElements/UIButton.h"

namespace UIInternalCalls
{
    // Rect transformation
    void RectTransformation_GetAnchorMin(EntityID entityID, glm::vec2* anchor);
    void RectTransformation_SetAnchorMin(EntityID entityID, glm::vec2* anchor);
    void RectTransformation_GetAnchorMax(EntityID entityID, glm::vec2* anchor);
    void RectTransformation_SetAnchorMax(EntityID entityID, glm::vec2* anchor);
    void RectTransformation_GetAnchoredPosition(EntityID entityID, glm::vec2* position);
    void RectTransformation_SetAnchoredPosition(EntityID entityID, glm::vec2* position);
    void RectTransformation_GetOffsetMin(EntityID entityID, glm::vec2* offset);
    void RectTransformation_SetOffsetMin(EntityID entityID, glm::vec2* offset);
    void RectTransformation_GetOffsetMax(EntityID entityID, glm::vec2* offset);
    void RectTransformation_SetOffsetMax(EntityID entityID, glm::vec2* offset);
    void RectTransformation_GetPivot(EntityID entityID, glm::vec2* pivot);
    void RectTransformation_SetPivot(EntityID entityID, glm::vec2* pivot);
    void RectTransformation_GetSize(EntityID entityID, glm::vec2* size);
    void RectTransformation_SetSize(EntityID entityID, glm::vec2* size);
    float RectTransformation_GetSortingOrder(EntityID entityID);
    void RectTransformation_SetSortingOrder(EntityID entityID, float sortingOrder);
    glm::vec3 RectTransformation_GetRotation(EntityID entityID);
    void RectTransformation_SetRotation(EntityID entityID, glm::vec3 rotation);

    // UI image
    ResourceID UIImage_GetSprite(EntityID entityID);
    void UIImage_SetSprite(EntityID entityID, ResourceID spriteID);

    // UI button
    ResourceID UIButton_GetSprite(EntityID entityID);
    void UIButton_SetSprite(EntityID entityID, ResourceID spriteID);
    bool UIButton_GetInteractable(EntityID entityID);
    void UIButton_SetInteractable(EntityID entityID, bool interactable);
    ButtonTransitionsInfo UIButton_GetTransition(EntityID entityID);
    void UIButton_SetTransition(EntityID entityID, ButtonTransitionsInfo info);

    // UI text
    MonoString* UIText_GetText(EntityID entityID);
    void UIText_SetText(EntityID entityID, MonoString* text);
    int UIText_GetTextSize(EntityID entityID);
    void UIText_SetTextSize(EntityID entityID, int textSize);
    glm::vec4 UIText_GetColor(EntityID entityID);
    void UIText_SetColor(EntityID entityID, glm::vec4 color);
    bool UIButton_GetIsTextAutoSize(EntityID entityID);
    void UIButton_SetIsTextAutoSize(EntityID entityID, bool isAutoSize);
    bool UIButton_GetTextAlignment(EntityID entityID);
    void UIButton_SetTextAlignment(EntityID entityID, AlignmentTypes::AlignmentType alignmentType);

    // UI
    EntityID UI_CreateUIElement();
    EntityID UI_CreateUIElement2(MonoString* name);
    EntityID UI_CreateUIElement3(MonoString* name, EntityID parentEntityID);
    bool UI_IsPointerOverUI();
}
