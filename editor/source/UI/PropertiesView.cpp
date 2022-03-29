#include "PropertiesView.h"
#include "HierarchyView.h"
#include "../EditorCore/EditorApplication.h"

#include <Steel.h>
#include <Steel/Common/StringUtils.h>

#define ELEMENT_SIZE 16.0f
#define Y_OFFSET 4.0f
#define X_OFFSET 8.0f
#define BIG_OFFSET 10.0f

#define ADD_TO_STRING_ELEMENT(m_type) \
{ \
    EntityID elementEntity = CreateSimpleStringElement( \
            entitiesRegistry, \
            layer, \
            StringUtils::ToString(componentNode[attribute.Name()].as<m_type>()), \
            attribute.Name(), \
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
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();

    EntityID frameEntity = layer->CreateUIImage(layer->UIResources.StraightFrameSprite, "Frame", Owner);
    auto& frameRT = entitiesRegistry->GetComponent<RectTransformation>(frameEntity);
    frameRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    frameRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
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
    ClearProperties(entitiesRegistry);

    if (entities.size() != 1)
        return;

    EntityID entityID = entities[0];

    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
    auto editor = (EditorApplication*)Application::Instance;
    auto appScene = editor->GetAppContext()->Scenes->GetActiveScene();

    SerializationContext context;
    context.SerializedScene = appScene;
    context.ResourcesSource = editor->GetAppContext()->Resources;

    float y = -Y_OFFSET;

    std::vector<RawComponentData> rawData;
    appScene->GetEntitiesRegistry()->GetAllComponentsForEntity(entityID, rawData);
    for (auto& dataPair : rawData)
    {
        auto object = static_cast<Serializable*>(dataPair.Data);
        auto typeInfo = TypeInfoStorage::GetTypeInfo(dataPair.TypeID);

        // Can skip some types (leave now for test)
        //if (typeInfo->ID == HierarchyNode::GetTypeInfo()->ID) continue;

        YAML::Node componentNode;
        SerializationManager::Serialize(typeInfo->ID, object, componentNode, context);

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
                    ADD_TO_STRING_ELEMENT(int)
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
                    ADD_TO_STRING_ELEMENT(float)
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
                        componentNode[attribute.Name()].as<std::string>(),
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
                            "<enum value " + StringUtils::ToString(componentNode[attribute.Name()].as<int>()) + ">",
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
                            "<list of size " + StringUtils::ToString(componentNode[attribute.Name()].size()) + ">",
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
}

void PropertiesView::ClearProperties(EntitiesRegistry* entitiesRegistry)
{
    for (auto entityID : uiElementsEntities)
    {
        entitiesRegistry->DeleteEntity(entityID);
    }
    uiElementsEntities.clear();
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
