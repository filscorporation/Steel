#include "UIInternalCalls.h"
#include "InternalCallsCommon.h"
#include "../UI/UIImage.h"
#include "../Core/Log.h"
#include "../Core/Application.h"
#include "../Scene/SceneHelper.h"

void UIInternalCalls::RectTransformation_GetAnchorMin(EntityID entityID, glm::vec2* anchor)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    anchor->x = component.GetAnchorMin().x;
    anchor->y = component.GetAnchorMin().y;
}

void UIInternalCalls::RectTransformation_SetAnchorMin(EntityID entityID, glm::vec2* anchor)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetAnchorMin(*anchor);
}

void UIInternalCalls::RectTransformation_GetAnchorMax(EntityID entityID, glm::vec2* anchor)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    anchor->x = component.GetAnchorMax().x;
    anchor->y = component.GetAnchorMax().y;
}

void UIInternalCalls::RectTransformation_SetAnchorMax(EntityID entityID, glm::vec2* anchor)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetAnchorMax(*anchor);
}

glm::vec3 UIInternalCalls::RectTransformation_GetAnchoredPosition(EntityID entityID)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, glm::vec3(0.0f))

    return component.GetAnchoredPosition();
}

void UIInternalCalls::RectTransformation_SetAnchoredPosition(EntityID entityID, glm::vec3 position)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetAnchoredPosition(position);
}

void UIInternalCalls::RectTransformation_GetOffsetMin(EntityID entityID, glm::vec2* offset)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    offset->x = component.GetOffsetMin().x;
    offset->y = component.GetOffsetMin().y;
}

void UIInternalCalls::RectTransformation_SetOffsetMin(EntityID entityID, glm::vec2* offset)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetOffsetMin(*offset);
}

void UIInternalCalls::RectTransformation_GetOffsetMax(EntityID entityID, glm::vec2* offset)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    offset->x = component.GetOffsetMax().x;
    offset->y = component.GetOffsetMax().y;
}

void UIInternalCalls::RectTransformation_SetOffsetMax(EntityID entityID, glm::vec2* offset)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetOffsetMax(*offset);
}

void UIInternalCalls::RectTransformation_GetPivot(EntityID entityID, glm::vec2* pivot)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    pivot->x = component.GetPivot().x;
    pivot->y = component.GetPivot().y;
}

void UIInternalCalls::RectTransformation_SetPivot(EntityID entityID, glm::vec2* pivot)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetPivot(*pivot);
}

void UIInternalCalls::RectTransformation_GetSize(EntityID entityID, glm::vec2* size)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    size->x = component.GetSize().x;
    size->y = component.GetSize().y;
}

void UIInternalCalls::RectTransformation_SetSize(EntityID entityID, glm::vec2* size)
{
    GET_COMPONENT_OR_RETURN(RectTransformation, )

    component.SetSize(*size);
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
