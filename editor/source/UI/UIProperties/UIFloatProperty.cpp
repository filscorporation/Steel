#include "UIFloatProperty.h"
#include "../PropertiesView.h"

#include <Steel.h>
#include <Steel/Common/StringUtils.h>

EntityID UIFloatProperty::Create(EntitiesRegistry* entitiesRegistry, UILayer* layer, ComponentNodeWrapper* componentNodeWrapper,
                                   const TypeInfo* typeInfo, const AttributeInfo& attributeInfo, float& x, float& y, EntityID parent)
{
    EntityID entity = layer->CreateUIElement("property", parent);

    auto& entityRT = entitiesRegistry->GetComponent<RectTransformation>(entity);
    entityRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
    entityRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    entityRT.SetSize(glm::vec2(0.0f, ELEMENT_SIZE));
    entityRT.SetPivot(glm::vec2(0.0f, 1.0f));
    entityRT.SetAnchoredPosition(glm::vec2(0, y));
    entityRT.SetOffsetMin(glm::vec2(x, 0.0f));

    const float SPLIT = 0.5f;

    {
        std::string label = attributeInfo.Label();
        EntityID labelTextEntity = layer->CreateUIText(label, "label", entity);

        auto& labelText = entitiesRegistry->GetComponent<UIText>(labelTextEntity);
        labelText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        auto& textRT = entitiesRegistry->GetComponent<RectTransformation>(labelTextEntity);
        textRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        textRT.SetAnchorMax(glm::vec2(SPLIT, 1.0f));
        textRT.SetSize(glm::vec2(0.0f, ELEMENT_SIZE));
        textRT.SetPivot(glm::vec2(0.0f, 1.0f));
        textRT.SetAnchoredPosition(glm::vec2(0, y));
        textRT.SetOffsetMin(glm::vec2(x, 0.0f));
    }

    {
        EntityID fieldEntity = layer->CreateUIInputField("field", entity);

        auto& field = entitiesRegistry->GetComponent<UIInputField>(fieldEntity);
        field.SetTextType(TextTypes::DecimalNumber);

        std::string attributeName = attributeInfo.Name();
        field.OnChangedCallback = [attributeName, componentNodeWrapper](EntityID entityID, const std::string& line)
        {
            componentNodeWrapper->Node[attributeName] = StringUtils::FromString<float>(line);
            componentNodeWrapper->IsDirty = true;
        };

        auto& fieldText = entitiesRegistry->GetComponent<UIText>(field.GetTargetText());
        fieldText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        fieldText.SetTextSize(16);
        std::string valueAsString = StringUtils::ToString(componentNodeWrapper->Node[attributeInfo.Name()].as<float>());
        fieldText.SetText(valueAsString);

        auto& textRT = entitiesRegistry->GetComponent<RectTransformation>(fieldEntity);
        textRT.SetAnchorMin(glm::vec2(SPLIT, 0.0f));
        textRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        textRT.SetSize(glm::vec2(0.0f, ELEMENT_SIZE));
        textRT.SetPivot(glm::vec2(0.0f, 1.0f));
        textRT.SetAnchoredPosition(glm::vec2(0, y));
        textRT.SetOffsetMin(glm::vec2(x, 0.0f));
        textRT.SetOffsetMax(glm::vec2(X_OFFSET, 0.0f));

        y -= ELEMENT_SIZE + Y_OFFSET;
    }

    return entity;
}
