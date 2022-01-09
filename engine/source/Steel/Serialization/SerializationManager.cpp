#include "SerializationManager.h"
#include "TypeInfoStorage.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/Core/Components.h"
#include "Steel/Resources/ResourcesManager.h"
#include "Steel/Scene/Scene.h"

std::unordered_map<ComponentTypeID, std::vector<AttributeInfo>> SerializationManager::_attributesInfo;

void SerializationManager::Init()
{
    RegisterComponents();

    Log::LogDebug("Serialization manager initialized");
}

void SerializationManager::Terminate()
{
    for (auto& classInfo : _attributesInfo)
    {
        for (auto& info : classInfo.second)
        {
            info.Clear();
        }
    }
    _attributesInfo.clear();

    TypeInfoStorage::Clear();
}

void SerializationManager::SerializeScene(Scene* scene, const std::string& filePath)
{
    YAML::Node node;
    std::string fullPathString = RESOURCES_PATH;
    fullPathString += filePath;

    if (!SerializeScene(scene, node))
    {
        Log::LogError("Error serializing scene");
        return;
    }

    std::ofstream fout(fullPathString);
    fout << node;

    Log::LogInfo("Saved scene " + filePath);
}

Scene* SerializationManager::DeserializeScene(const std::string& filePath)
{
    std::string fullPathString = RESOURCES_PATH;
    fullPathString += filePath;

    std::ifstream infile(fullPathString);
    if (!infile.good())
    {
        Log::LogError("Error loading scene: file {0} does not exist", fullPathString);
        return nullptr;
    }

    YAML::Node node = YAML::Load(infile);
    auto scene = new Scene("");

    if (!DeserializeScene(scene, node))
    {
        Log::LogError("Error serializing scene");
        return nullptr;
    }

    Log::LogInfo("Loaded scene " + filePath);

    return scene;
}

void SerializationManager::RegisterAttribute(ComponentTypeID classTypeID, const AttributeInfo& attributeInfo)
{
    _attributesInfo[classTypeID].emplace_back(attributeInfo);
}

std::vector<AttributeInfo>& SerializationManager::GetAttributes(ComponentTypeID classTypeID)
{
    return _attributesInfo[classTypeID];
}

bool SerializationManager::SerializeScene(Scene* scene, YAML::Node& node)
{
    SerializationContext context;
    context.SerializedScene = scene;
    context.ResourcesSource = Application::Context()->Resources;

    node["name"] = scene->_name;
    node["mainCamera"] = scene->_mainCameraEntity;
    node["childrenCount"] = scene->GetChildrenCount();
    node["firstChildNode"] = scene->GetFirstChildNode();

    auto entitiesNode = node["entities"];

    for (int i = 0; i < scene->entitiesRegistry->EntityGetMaxID(); ++i)
    {
        EntityID entity = scene->entitiesRegistry->EntityActual(i);

        if (scene->entitiesRegistry->EntityExists(entity))
        {
            YAML::Node entityNode;
            entityNode["uuid"] = scene->entitiesRegistry->HasComponent<IDComponent>(entity)
                    ? scene->entitiesRegistry->GetComponent<IDComponent>(entity).GetUUID()
                    : NULL_UUID;
            entityNode["state"] = (uint32_t)scene->entitiesRegistry->EntityGetState(entity);
            YAML::Node componentsNode = entityNode["components"];

            std::vector<std::pair<ComponentTypeID, void*>> rawData;
            scene->entitiesRegistry->GetAllComponentsForEntity(entity, rawData);
            for (auto dataPair : rawData)
            {
                auto object = static_cast<Serializable*>(dataPair.second);
                auto typeInfo = scene->entitiesRegistry->GetTypeInfoByID(dataPair.first);

                YAML::Node componentNode;

                if (_attributesInfo.find(typeInfo->ID) != _attributesInfo.end())
                {
                    for (auto attribute : _attributesInfo[typeInfo->ID])
                    {
                        attribute.Serialize(object, componentNode, context);
                    }
                }

                componentsNode[typeInfo->TypeName] = componentNode;
            }

            entitiesNode.push_back(entityNode);
        }
    }

    return true;
}

bool SerializationManager::DeserializeScene(Scene* scene, YAML::Node& node)
{
    SerializationContext context;
    context.SerializedScene = scene;
    context.ResourcesSource = Application::Context()->Resources;

    scene->_name = node["name"].as<std::string>();
    scene->_mainCameraEntity = node["mainCamera"].as<EntityID>();
    scene->SetChildrenCount(node["childrenCount"].as<uint32_t>());
    scene->SetFirstChildNode(node["firstChildNode"].as<EntityID>());

    auto entitiesNode = node["entities"];
    Log::LogInfo("Entities count loaded: {0}", entitiesNode.size());
    if (entitiesNode)
    {
        // Pre pass to fill UUID to EntityID map (for link attributes)
        for (auto entityNode : entitiesNode)
        {
            auto entity = scene->entitiesRegistry->CreateNewEntity();
            auto uuid = entityNode["uuid"].as<UUID>();
            if (uuid != NULL_UUID)
                scene->SetEntityByUUID(uuid, entity);
        }

        // Main pass
        for (auto entityNode : entitiesNode)
        {
            auto uuid = entityNode["uuid"].as<UUID>();
            auto entity = scene->GetEntityByUUID(uuid);
            if (entity == NULL_ENTITY)
                entity = scene->entitiesRegistry->CreateNewEntity();
            auto entityState = (EntityStates::EntityState)entityNode["state"].as<uint32_t>();

            if (!scene->entitiesRegistry->RestoreEntityState(entity, entityState))
            {
                Log::LogError("Entity {0} state was not restored correctly", entity);
                continue;
            }

            auto componentsNode = entityNode["components"];
            if (componentsNode)
            {
                for (auto componentNode : componentsNode)
                {
                    auto typeName = componentNode.first.as<std::string>();
                    auto typeID = Math::StringHash(typeName.c_str());

                    // This will add component map to registry, so we can add component of deserialized type without template arguments
                    if (!TypeInfoStorage::CheckTypeRegistered(typeID, scene->entitiesRegistry))
                    {
                        Log::LogError("Type {0} not registered", typeName);
                        continue;
                    }
                    auto object = static_cast<Serializable*>(scene->entitiesRegistry->RestoreComponent(typeID, entity));
                    if (!object)
                    {
                        Log::LogError("Object of type {0} was not restored properly", typeName);
                        continue;
                    }

                    if (_attributesInfo.find(typeID) != _attributesInfo.end())
                    {
                        for (auto attribute : _attributesInfo[typeID])
                        {
                            attribute.Deserialize(object, componentNode.second, context);
                        }
                    }

                    // TODO: find out if we need to call OnCreate or something like this after attributes initialized
                    // (required for components like UIEventHandler (from UIButton and other UI elements))
                }
            }
        }
    }

    return true;
}

void SerializationManager::RegisterComponents()
{
    Animator::RegisterType();
    AudioListener::RegisterType();
    AudioSource::RegisterType();
    BoxCollider::RegisterType();
    CircleCollider::RegisterType();
    RigidBody::RegisterType();
    Camera::RegisterType();
    MeshRenderer::RegisterType();
    SpriteRenderer::RegisterType();
    HierarchyNode::RegisterType();
    IDComponent::RegisterType();
    NameComponent::RegisterType();
    Transformation::RegisterType();
    ScriptComponent::RegisterType();
    UIButton::RegisterType();
    UICheckBox::RegisterType();
    UIClipping::RegisterType();
    UIImage::RegisterType();
    UIInputField::RegisterType();
    UILayoutElement::RegisterType();
    UILayoutGroup::RegisterType();
    UITabs::RegisterType();
    UIText::RegisterType();
    RectTransformation::RegisterType();
    UIEventHandler::RegisterType();
}
