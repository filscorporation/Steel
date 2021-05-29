#include <mono/jit/jit.h>

#include "UIInternalCalls.h"
#include "InternalCallsCommon.h"
#include "../Core/Log.h"
#include "../Core/Application.h"
#include "../Scene/SceneHelper.h"
#include "../UI/UIEventHandler.h"
#include "../UI/UIElements/UIButton.h"
#include "../UI/UIElements/UIText.h"

void UIInternalCalls::RectTransformation_GetAnchorMin(EntityID entityID, glm::vec2* anchor)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    glm::vec2 result = component.GetAnchorMin();
    anchor->x = result.x;
    anchor->y = result.y;
}

void UIInternalCalls::RectTransformation_SetAnchorMin(EntityID entityID, glm::vec2* anchor)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetAnchorMin(*anchor);
}

void UIInternalCalls::RectTransformation_GetAnchorMax(EntityID entityID, glm::vec2* anchor)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    glm::vec2 result = component.GetAnchorMax();
    anchor->x = result.x;
    anchor->y = result.y;
}

void UIInternalCalls::RectTransformation_SetAnchorMax(EntityID entityID, glm::vec2* anchor)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetAnchorMax(*anchor);
}

void UIInternalCalls::RectTransformation_GetAnchoredPosition(EntityID entityID, glm::vec2* position)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    glm::vec2 result = component.GetAnchoredPosition();
    position->x = result.x;
    position->y = result.y;
}

void UIInternalCalls::RectTransformation_SetAnchoredPosition(EntityID entityID, glm::vec2* position)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetAnchoredPosition(*position);
}

void UIInternalCalls::RectTransformation_GetOffsetMin(EntityID entityID, glm::vec2* offset)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    glm::vec2 result = component.GetOffsetMin();
    offset->x = result.x;
    offset->y = result.y;
}

void UIInternalCalls::RectTransformation_SetOffsetMin(EntityID entityID, glm::vec2* offset)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetOffsetMin(*offset);
}

void UIInternalCalls::RectTransformation_GetOffsetMax(EntityID entityID, glm::vec2* offset)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    glm::vec2 result = component.GetOffsetMax();
    offset->x = result.x;
    offset->y = result.y;
}

void UIInternalCalls::RectTransformation_SetOffsetMax(EntityID entityID, glm::vec2* offset)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetOffsetMax(*offset);
}

void UIInternalCalls::RectTransformation_GetPivot(EntityID entityID, glm::vec2* pivot)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    glm::vec2 result = component.GetPivot();
    pivot->x = result.x;
    pivot->y = result.y;
}

void UIInternalCalls::RectTransformation_SetPivot(EntityID entityID, glm::vec2* pivot)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetPivot(*pivot);
}

void UIInternalCalls::RectTransformation_GetSize(EntityID entityID, glm::vec2* size)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    glm::vec2 result = component.GetSize();
    size->x = result.x;
    size->y = result.y;
}

void UIInternalCalls::RectTransformation_SetSize(EntityID entityID, glm::vec2* size)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetSize(*size);
}

float UIInternalCalls::RectTransformation_GetSortingOrder(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, 0.0f)

    return component.GetSortingOrder();
}

void UIInternalCalls::RectTransformation_SetSortingOrder(EntityID entityID, float sortingOrder)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetSortingOrder(sortingOrder);
}

glm::vec3 UIInternalCalls::RectTransformation_GetRotation(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, glm::vec3(0.0f))

    return component.GetRotation();
}

void UIInternalCalls::RectTransformation_SetRotation(EntityID entityID, glm::vec3 rotation)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetRotation(rotation);
}

ResourceID UIInternalCalls::UIImage_GetSprite(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIImage, 0)
    auto image = component.GetImage();

    return image == nullptr ? 0 : image->ID;
}

void UIInternalCalls::UIImage_SetSprite(EntityID entityID, ResourceID spriteID)
{
    GET_COMPONENT_OR_RETURN(UIImage, )

    component.SetImage(Application::Instance->GetResourcesManager()->GetImage(spriteID));
}

glm::vec4 UIInternalCalls::UIImage_GetColor(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIImage, glm::vec4(0.0f))

    return component.GetColor();
}

void UIInternalCalls::UIImage_SetColor(EntityID entityID, glm::vec4 color)
{
    GET_COMPONENT_OR_RETURN(UIImage, )

    component.SetColor(color);
}

bool UIInternalCalls::UIImage_GetConsumeEvents(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIEventHandler, 0)

    return !component.IsTransparent;
}

void UIInternalCalls::UIImage_SetConsumeEvents(EntityID entityID, bool consume)
{
    GET_COMPONENT_OR_RETURN(UIEventHandler, )

    component.IsTransparent = !consume;
}

ResourceID UIInternalCalls::UIButton_GetTargetImage(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIButton, 0)

    return component.GetTargetImage();
}

void UIInternalCalls::UIButton_SetTargetImage(EntityID entityID, EntityID targetID)
{
    GET_COMPONENT_OR_RETURN(UIButton, )

    component.SetTargetImage(targetID);
}

bool UIInternalCalls::UIButton_GetInteractable(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIButton, false)

    return component.GetInteractable();
}

void UIInternalCalls::UIButton_SetInteractable(EntityID entityID, bool interactable)
{
    GET_COMPONENT_OR_RETURN(UIButton, )

    component.SetInteractable(interactable);
}

ButtonTransitionsInfo UIInternalCalls::UIButton_GetTransition(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIButton, ButtonTransitionsInfo())

    return component.GetTransitionsInfo();
}

void UIInternalCalls::UIButton_SetTransition(EntityID entityID, ButtonTransitionsInfo info)
{
    GET_COMPONENT_OR_RETURN(UIButton, )

    component.SetTransitionsInfo(info);
}

MonoString* UIInternalCalls::UIText_GetText(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIText, nullptr)

    return mono_string_new(mono_domain_get(), component.GetText().c_str());
}

void UIInternalCalls::UIText_SetText(EntityID entityID, MonoString* text)
{
    GET_COMPONENT_OR_RETURN(UIText, )

    component.SetText(mono_string_to_utf8(text));
}

int UIInternalCalls::UIText_GetTextSize(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIText, 0)

    return component.GetTextSize();
}

void UIInternalCalls::UIText_SetTextSize(EntityID entityID, int textSize)
{
    GET_COMPONENT_OR_RETURN(UIText, )

    component.SetTextSize(textSize);
}

float UIInternalCalls::UIText_GetLineSpacing(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIText, 0.0f)

    return component.GetLineSpacing();
}

void UIInternalCalls::UIText_SetLineSpacing(EntityID entityID, float spacing)
{
    GET_COMPONENT_OR_RETURN(UIText, )

    component.SetLineSpacing(spacing);
}

glm::vec4 UIInternalCalls::UIText_GetColor(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIText, glm::vec4(0.0f))

    return component.GetColor();
}

void UIInternalCalls::UIText_SetColor(EntityID entityID, glm::vec4 color)
{
    GET_COMPONENT_OR_RETURN(UIText, )

    component.SetColor(color);
}

bool UIInternalCalls::UIButton_GetIsTextAutoSize(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIText, 0)

    return component.GetIsTextAutoSize();
}

void UIInternalCalls::UIButton_SetIsTextAutoSize(EntityID entityID, bool isAutoSize)
{
    GET_COMPONENT_OR_RETURN(UIText, )

    component.SetIsTextAutoSize(isAutoSize);
}

AlignmentTypes::AlignmentType UIInternalCalls::UIButton_GetTextAlignment(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIText, AlignmentTypes::CenterLeft)

    return component.GetTextAlignment();
}

void UIInternalCalls::UIButton_SetTextAlignment(EntityID entityID, AlignmentTypes::AlignmentType alignmentType)
{
    GET_COMPONENT_OR_RETURN(UIText, )

    component.SetTextAlignment(alignmentType);
}

OverflowModes::OverflowMode UIInternalCalls::UIButton_GetTextOverflowMode(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIText, OverflowModes::Overflow)

    return component.GetOverflowMode();
}

void UIInternalCalls::UIButton_SetTextOverflowMode(EntityID entityID, OverflowModes::OverflowMode overflowMode)
{
    GET_COMPONENT_OR_RETURN(UIText, )

    component.SetOverflowMode(overflowMode);
}

EntityID UIInternalCalls::UIInputField_GetTargetText(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIInputField, 0)

    return component.GetTargetText();
}

void UIInternalCalls::UIInputField_SetTargetText(EntityID entityID, EntityID targetID)
{
    GET_COMPONENT_OR_RETURN(UIInputField, )

    component.SetTargetText(targetID);
}

EntityID UIInternalCalls::UIInputField_GetTargetImage(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(UIInputField, 0)

    return component.GetTargetImage();
}

void UIInternalCalls::UIInputField_SetTargetImage(EntityID entityID, EntityID targetID)
{
    GET_COMPONENT_OR_RETURN(UIInputField, )

    component.SetTargetImage(targetID);
}

EntityID UIInternalCalls::UI_CreateUIElement()
{
    return Application::Instance->GetCurrentScene()->GetUILayer()->CreateUIElement();
}

EntityID UIInternalCalls::UI_CreateUIElement2(MonoString* name)
{
    return Application::Instance->GetCurrentScene()->GetUILayer()->CreateUIElement(mono_string_to_utf8(name), NULL_ENTITY);
}

EntityID UIInternalCalls::UI_CreateUIElement3(MonoString* name, EntityID parentEntityID)
{
    return Application::Instance->GetCurrentScene()->GetUILayer()->CreateUIElement(mono_string_to_utf8(name), parentEntityID);
}

EntityID UIInternalCalls::UI_CreateUIImage()
{
    return Application::Instance->GetCurrentScene()->GetUILayer()->CreateUIImage();
}

EntityID UIInternalCalls::UI_CreateUIImage2(ResourceID spriteID, MonoString* name, EntityID parentEntityID)
{
    Sprite* sprite = Application::Instance->GetResourcesManager()->GetImage(spriteID);

    return Application::Instance->GetCurrentScene()->GetUILayer()->CreateUIImage(sprite, mono_string_to_utf8(name), parentEntityID);
}

EntityID UIInternalCalls::UI_CreateUIButton()
{
    return Application::Instance->GetCurrentScene()->GetUILayer()->CreateUIButton();
}

EntityID UIInternalCalls::UI_CreateUIButton2(ResourceID spriteID, MonoString* name, EntityID parentEntityID)
{
    Sprite* sprite = Application::Instance->GetResourcesManager()->GetImage(spriteID);

    return Application::Instance->GetCurrentScene()->GetUILayer()->CreateUIButton(sprite, mono_string_to_utf8(name), parentEntityID);
}

EntityID UIInternalCalls::UI_CreateUIText()
{
    return Application::Instance->GetCurrentScene()->GetUILayer()->CreateUIText();
}

EntityID UIInternalCalls::UI_CreateUIText2(MonoString* text, MonoString* name, EntityID parentEntityID)
{
    return Application::Instance->GetCurrentScene()->GetUILayer()->CreateUIText(mono_string_to_utf8(text), mono_string_to_utf8(name), parentEntityID);
}

EntityID UIInternalCalls::UI_CreateUIInputField()
{
    return Application::Instance->GetCurrentScene()->GetUILayer()->CreateUIInputField();
}

EntityID UIInternalCalls::UI_CreateUIInputField2(MonoString* name, EntityID parentEntityID)
{
    return Application::Instance->GetCurrentScene()->GetUILayer()->CreateUIInputField(mono_string_to_utf8(name), parentEntityID);
}

bool UIInternalCalls::UI_IsPointerOverUI()
{
    return Application::Instance->GetCurrentScene()->GetUILayer()->IsPointerOverUI();
}
