#include "PropertiesView.h"
#include "HierarchyView.h"
#include "../EditorCore/EditorApplication.h"
#include "UIProperties/UIIntegerProperty.h"
#include "UIProperties/UIFloatProperty.h"

#include <Steel.h>
#include <Steel/Common/StringUtils.h>
#include <Steel/Serialization/AttributesRegistration.h>

#define ADD_TO_STRING_ELEMENT(m_type) \
{ \
    EntityID elementEntity = CreateSimpleStringElement( \
            entitiesRegistry, \
            layer, \
            StringUtils::ToString(componentNodeWrapper->Node[attribute.Name()].as<m_type>()), \
            attribute.Label(), \
            x, \
            y \
    ); \
     \
    uiElementsEntities.push_back(elementEntity); \
    break; \
}

void PropertiesView::RegisterType()
{
    REGISTER_COMPONENT(PropertiesView);
}

void PropertiesView::Init(EntitiesRegistry* entitiesRegistry)
{

}

void PropertiesView::Update(EntitiesRegistry* entitiesRegistry)
{
    std::vector<EntityID> selectedEntities;
    auto hierarchyViewIterator = entitiesRegistry->GetComponentIterator<HierarchyView>();
    for (int i = 0; i < hierarchyViewIterator.Size(); ++i)
    {
        if (hierarchyViewIterator[i].IsAlive())
        {
            hierarchyViewIterator[i].GetSelectedEntities(entitiesRegistry, selectedEntities);
            DrawProperties(entitiesRegistry, selectedEntities);
            break;
        }
    }
}

void PropertiesView::DrawProperties(EntitiesRegistry* entitiesRegistry, std::vector<EntityID>& entities)
{
    if (entities.size() != 1)
        return;

    EntityID entityID = entities[0];

    if (entityID != lastEntityID)
    {
        ClearProperties(entitiesRegistry);
        isDirty = true;
        lastEntityID = entityID;
    }

    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
    auto editor = (EditorApplication*)Application::Instance;
    auto appScene = editor->GetAppContext()->Scenes->GetActiveScene();

    SerializationContext context;
    context.SerializedScene = appScene;
    context.ResourcesSource = editor->GetAppContext()->Resources;

    float y = -Y_OFFSET;

    std::vector<RawComponentData> rawData;
    appScene->GetEntitiesRegistry()->GetAllComponentsForEntity(entityID, rawData);

    // TODO: compare with previous
    if (!isDirty)
    {
        editor->SwitchContext(editor->AppContext);

        // Apply data changed by properties
        for (auto& componentNodeWrapper : componentsNodes)
        {
            if (componentNodeWrapper->IsDirty)
            {
                // TODO: if we set dirty here - this will rebuild UI stopping all the animations and editing
                // TODO: if not - fields changing other fields will get updated in UI
                // TODO: best solution if to rebuild UI partly
                //isDirty = true;

                // TODO: keep in mind that between PropertyView catches event and this place some fields can be modified,
                // TODO: so applying data from node to object here can overwrite some external changes

                auto typeInfo = TypeInfoStorage::GetTypeInfo(componentNodeWrapper->ComponentTypeID);
                auto typeID = componentNodeWrapper->ComponentTypeID;

                // TODO: linear search is bad
                int rawComponentIndex = -1;
                for (int i = 0; i < rawData.size(); i++)
                {
                    if (rawData[i].DataTypeID == typeID)
                    {
                        rawComponentIndex = i;
                        break;
                    }
                }

                if (rawComponentIndex == -1)
                {
                    Log::LogError("Component of type {0} was changed by view, but was not found in deserialized data", typeInfo->TypeName);
                    continue;
                }

                auto object = static_cast<Serializable*>(rawData[rawComponentIndex].Data);
                SerializationManager::Deserialize(typeID, object, componentNodeWrapper->Node, context);
            }
        }

        editor->SwitchContext(editor->EditorContext);
    }

    if (!isDirty)
        return;
    isDirty = false;

    ClearProperties(entitiesRegistry);

    for (auto& dataPair : rawData)
    {
        auto object = static_cast<Serializable*>(dataPair.Data);
        auto typeInfo = TypeInfoStorage::GetTypeInfo(dataPair.DataTypeID);

        // Can skip some types (leave now for test)
        //if (typeInfo->ID == HierarchyNode::GetTypeInfo()->ID) continue;

        auto componentNodeWrapper = new ComponentNodeWrapper();
        componentNodeWrapper->ComponentTypeID = typeInfo->ID;
        componentsNodes.push_back(componentNodeWrapper);
        SerializationManager::Serialize(typeInfo->ID, object, componentNodeWrapper->Node, context);

        float x = X_OFFSET;

        // Component type name
        {
            std::string text = "[" + std::string(typeInfo->TypeName) + "]";
            EntityID elementEntity = layer->CreateUIText(text, "node", Owner);

            auto& elementText = entitiesRegistry->GetComponent<UIText>(elementEntity);
            elementText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

            auto& elementRT = entitiesRegistry->GetComponent<RectTransformation>(elementEntity);
            elementRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
            elementRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
            elementRT.SetSize(glm::vec2(0.0f, ELEMENT_SIZE));
            elementRT.SetPivot(glm::vec2(0.0f, 1.0f));
            elementRT.SetAnchoredPosition(glm::vec2(0, y));
            elementRT.SetOffsetMin(glm::vec2(x, 0.0f));

            y -= ELEMENT_SIZE + Y_OFFSET;
            uiElementsEntities.push_back(elementEntity);
        }

        x += X_OFFSET;

        auto& attributes = SerializationManager::GetAttributes(typeInfo->ID);
        for (auto& attribute : attributes)
        {
            switch (attribute.GetType())
            {
                case Types::Undefined:
                {
                    EntityID elementEntity = CreateSimpleStringElement(
                        entitiesRegistry,
                        layer,
                        "<undefined>",
                        attribute.Name(),
                        x,
                        y
                    );

                    uiElementsEntities.push_back(elementEntity);
                    break;
                }
                case Types::Int:
                {
                    EntityID elementEntity = UIIntegerProperty::Create(
                            entitiesRegistry,
                            layer,
                            componentNodeWrapper,
                            typeInfo,
                            attribute,
                            x,
                            y,
                            Owner
                    );

                    uiElementsEntities.push_back(elementEntity);
                    break;
                }
                case Types::Long:
                    ADD_TO_STRING_ELEMENT(long)
                case Types::UInt32:
                    ADD_TO_STRING_ELEMENT(uint32_t)
                case Types::UInt64:
                    ADD_TO_STRING_ELEMENT(uint64_t)
                case Types::Char:
                    ADD_TO_STRING_ELEMENT(char)
                case Types::Bool:
                    ADD_TO_STRING_ELEMENT(bool)
                case Types::Float:
                {
                    EntityID elementEntity = UIFloatProperty::Create(
                            entitiesRegistry,
                            layer,
                            componentNodeWrapper,
                            typeInfo,
                            attribute,
                            x,
                            y,
                            Owner
                    );

                    uiElementsEntities.push_back(elementEntity);
                    break;
                }
                case Types::Double:
                    ADD_TO_STRING_ELEMENT(double)
                case Types::Vector2:
                    ADD_TO_STRING_ELEMENT(glm::vec2)
                case Types::Vector3:
                    ADD_TO_STRING_ELEMENT(glm::vec3)
                case Types::Color:
                    ADD_TO_STRING_ELEMENT(glm::vec4)
                case Types::String:
                {
                    EntityID elementEntity = CreateSimpleStringElement(
                            entitiesRegistry,
                            layer,
                            componentNodeWrapper->Node[attribute.Name()].as<std::string>(),
                            attribute.Name(),
                            x,
                            y
                    );

                    uiElementsEntities.push_back(elementEntity);
                    break;
                }
                case Types::Enum:
                {
                    EntityID elementEntity = CreateSimpleStringElement(
                            entitiesRegistry,
                            layer,
                            "<enum value " + StringUtils::ToString(componentNodeWrapper->Node[attribute.Name()].as<int>()) + ">",
                            attribute.Name(),
                            x,
                            y
                    );

                    uiElementsEntities.push_back(elementEntity);
                    break;
                }
                case Types::EntityID:
                    ADD_TO_STRING_ELEMENT(UUID_TYPE)
                case Types::ResourceID:
                    ADD_TO_STRING_ELEMENT(UUID_TYPE)
                case Types::List:
                {
                    EntityID elementEntity = CreateSimpleStringElement(
                            entitiesRegistry,
                            layer,
                            "<list of size " + StringUtils::ToString(componentNodeWrapper->Node[attribute.Name()].size()) + ">",
                            attribute.Name(),
                            x,
                            y
                    );

                    uiElementsEntities.push_back(elementEntity);
                    break;
                }
                case Types::Script:
                {
                    EntityID elementEntity = CreateSimpleStringElement(
                            entitiesRegistry,
                            layer,
                            "<scripts>",
                            attribute.Name(),
                            x,
                            y
                    );

                    uiElementsEntities.push_back(elementEntity);
                    break;
                }
            }
        }

        y -= BIG_OFFSET;
    }

    entitiesRegistry->GetComponent<RectTransformation>(Owner).SetSize(glm::vec2(0.0f, -y));
}

void PropertiesView::ClearProperties(EntitiesRegistry* entitiesRegistry)
{
    for (auto entityID : uiElementsEntities)
    {
        entitiesRegistry->DeleteEntity(entityID);
    }
    uiElementsEntities.clear();

    for (auto nodeWrapper : componentsNodes)
    {
        delete nodeWrapper;
    }
    componentsNodes.clear();

    entitiesRegistry->GetComponent<RectTransformation>(Owner).SetAnchoredPosition(glm::vec2(0.0f, 0.0f)); // or maybe remember
}

EntityID PropertiesView::CreateSimpleStringElement(
        EntitiesRegistry* entitiesRegistry,
        UILayer* layer,
        const std::string& text,
        const std::string& name,
        float& x,
        float& y
    )
{
    std::string fullText = name + ": " + text;
    EntityID elementEntity = layer->CreateUIText(fullText, "node", Owner);

    auto& elementText = entitiesRegistry->GetComponent<UIText>(elementEntity);
    elementText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    auto& elementRT = entitiesRegistry->GetComponent<RectTransformation>(elementEntity);
    elementRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
    elementRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    elementRT.SetSize(glm::vec2(0.0f, ELEMENT_SIZE));
    elementRT.SetPivot(glm::vec2(0.0f, 1.0f));
    elementRT.SetAnchoredPosition(glm::vec2(0, y));
    elementRT.SetOffsetMin(glm::vec2(x, 0.0f));

    y -= ELEMENT_SIZE + Y_OFFSET;

    return elementEntity;
}
