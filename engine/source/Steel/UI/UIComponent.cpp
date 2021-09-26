#include "UIComponent.h"
#include "RectTransformation.h"
#include "../Core/Log.h"

bool UIComponent::CheckRectTransformation(EntitiesRegistry* entitiesRegistry)
{
    if (!entitiesRegistry->HasComponent<RectTransformation>(Owner))
    {
        Log::LogError("Adding UIComponents to objects without RectTransformation will lead to undefined behaviour. "
                      "Create UI objects using UILayout, not Scene");

        return false;
    }

    return true;
}
