#include "PropertiesView.h"
#include "HierarchyView.h"
#include "../EditorCore/EditorApplication.h"
#include "UIProperties/UIIntegerProperty.h"
#include "UIProperties/UIFloatProperty.h"
#include "ProjectView.h"

#include <Steel.h>
#include <Steel/Common/StringUtils.h>
#include <Steel/Serialization/AttributesRegistration.h>

#define ADD_TO_STRING_ELEMENT(m_type) \
{ \
    EntityID elementEntity = CreateSimpleStringElement( \
            entitiesRegistry, \
            layer, \
            StringUtils::ToString(typeNodeWrapper->Node[attribute.Name()].as<m_type>()), \
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
    DrawSelected(entitiesRegistry);
}

void PropertiesView::DrawSelected(EntitiesRegistry* entitiesRegistry)
{
    std::vector<EntityID> selectedEntities;
    auto hierarchyViewIterator = entitiesRegistry->GetComponentIterator<HierarchyView>();
    for (int i = 0; i < hierarchyViewIterator.Size(); ++i)
    {
        if (hierarchyViewIterator[i].IsAlive())
        {
            hierarchyViewIterator[i].GetSelectedEntities(entitiesRegistry, selectedEntities);
            if (!selectedEntities.empty())
            {
                DrawEntityProperties(entitiesRegistry, selectedEntities);
                return;
            }
        }
    }

    std::vector<std::string> selectedFiles;
    auto projectViewIterator = entitiesRegistry->GetComponentIterator<ProjectView>();
    for (int i = 0; i < projectViewIterator.Size(); ++i)
    {
        if (projectViewIterator[i].IsAlive())
        {
            projectViewIterator[i].GetSelectedFiles(entitiesRegistry, selectedFiles);
            if (!selectedFiles.empty())
            {
                DrawResourceProperties(entitiesRegistry, selectedFiles);
                return;
            }
        }
    }
}

void PropertiesView::DrawEntityProperties(EntitiesRegistry* entitiesRegistry, std::vector<EntityID>& entities)
{
    if (entities.size() != 1)
        return;

    EntityID entityID = entities[0];

    if (entityID != lastEntityID)
    {
        ClearProperties(entitiesRegistry);
        isDirty = true;
        lastEntityID = entityID;
        lastFilePath = "";
    }

    auto editor = (EditorApplication*)Application::Instance;
    auto appScene = editor->GetAppContext()->Scenes->GetActiveScene();

    std::vector<RawComponentData> rawData;
    appScene->GetEntitiesRegistry()->GetAllComponentsForEntity(entityID, rawData);

    std::vector<Serializable*> data;
    for (auto rawDataElement : rawData)
    {
        auto object = static_cast<Serializable*>(rawDataElement.Data);
        data.emplace_back(object);
    }

    DrawProperties(entitiesRegistry, data);
}

void PropertiesView::DrawResourceProperties(EntitiesRegistry* entitiesRegistry, const std::vector<std::string>& files)
{
    if (files.size() != 1)
        return;

    auto& filePath = files[0];

    if (filePath != lastFilePath)
    {
        ClearProperties(entitiesRegistry);
        isDirty = true;
        lastFilePath = filePath;
        lastEntityID = NULL_ENTITY;
    }

    auto editor = (EditorApplication*)Application::Instance;
    auto appResources = editor->GetAppContext()->Resources;

    Resource* resource = appResources->GetResource(ResourcesManager::GetAbsolutePath(filePath));

    std::vector<Serializable*> data;
    data.emplace_back(resource);

    DrawProperties(entitiesRegistry, data);
}

void PropertiesView::ClearProperties(EntitiesRegistry* entitiesRegistry)
{
    for (auto entityID : uiElementsEntities)
    {
        entitiesRegistry->DeleteEntity(entityID);
    }
    uiElementsEntities.clear();

    for (auto nodeWrapper : typesNodes)
    {
        delete nodeWrapper;
    }
    typesNodes.clear();

    entitiesRegistry->GetComponent<RectTransformation>(Owner).SetAnchoredPosition(glm::vec2(0.0f, 0.0f)); // or maybe remember
}

void PropertiesView::DrawProperties(EntitiesRegistry* entitiesRegistry, const std::vector<Serializable*>& data)
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
    auto editor = (EditorApplication*)Application::Instance;
    auto appScene = editor->GetAppContext()->Scenes->GetActiveScene();

    SerializationContext context;
    context.SerializedScene = appScene;
    context.ResourcesSource = editor->GetAppContext()->Resources;

    float y = -Y_OFFSET;

    // TODO: compare with previous
    if (!isDirty)
    {
        editor->SwitchContext(editor->AppContext);

        // Apply data changed by properties
        for (auto& typeNodeWrapper : typesNodes)
        {
            if (typeNodeWrapper->IsDirty)
            {
                // TODO: if we set dirty here - this will rebuild UI stopping all the animations and editing
                // TODO: if not - fields changing other fields will get updated in UI
                // TODO: best solution if to rebuild UI partly
                //isDirty = true;

                // TODO: keep in mind that between PropertyView catches event and this place some fields can be modified,
                // TODO: so applying data from node to object here can overwrite some external changes

                auto typeID = typeNodeWrapper->DataTypeID;

                // TODO: linear search is bad
                int dataTypeIndex = -1;
                for (int i = 0; i < data.size(); i++)
                {
                    if (data[i]->GetTypeInfo()->ID == typeID)
                    {
                        dataTypeIndex = i;
                        break;
                    }
                }

                if (dataTypeIndex == -1)
                {
                    Log::LogError("Data of type {0} was changed by view, but was not found in deserialized data", typeID);
                    continue;
                }

                SerializationManager::Deserialize(typeID, data[dataTypeIndex], typeNodeWrapper->Node, context);
            }
        }

        editor->SwitchContext(editor->EditorContext);
    }

    if (!isDirty)
        return;
    isDirty = false;

    ClearProperties(entitiesRegistry);

    for (auto& object : data)
    {
        if (object == nullptr)
            continue;

        auto typeInfo = object->GetTypeInfo();

        // Can skip some types (leave now for test)
        //if (typeInfo->ID == HierarchyNode::GetTypeInfo()->ID) continue;

        auto typeNodeWrapper = new TypeNodeWrapper();
        typeNodeWrapper->DataTypeID = typeInfo->ID;
        typesNodes.push_back(typeNodeWrapper);
        SerializationManager::Serialize(typeInfo->ID, object, typeNodeWrapper->Node, context);

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
                            typeNodeWrapper,
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
                            typeNodeWrapper,
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
                            typeNodeWrapper->Node[attribute.Name()].as<std::string>(),
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
                            "<enum value " + StringUtils::ToString(typeNodeWrapper->Node[attribute.Name()].as<int>()) + ">",
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
                            "<list of size " + StringUtils::ToString(typeNodeWrapper->Node[attribute.Name()].size()) + ">",
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
