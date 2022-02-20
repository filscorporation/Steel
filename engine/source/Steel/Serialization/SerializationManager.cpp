#include "SerializationManager.h"
#include "Steel/Core/Application.h"
#include "Steel/Core/Log.h"
#include "Steel/EntityComponentSystem/TypeInfoStorage.h"
#include "Steel/Resources/ResourcesManager.h"
#include "Steel/Scene/Scene.h"

std::unordered_map<ComponentTypeID, std::vector<AttributeInfo>> SerializationManager::_attributesInfo;

YAML::Node* SerializationManager::_sceneBackup = nullptr;

void SerializationManager::Init()
{
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

    delete _sceneBackup;
    _sceneBackup = nullptr;
}

void SerializationManager::SerializeScene(Scene* scene, const std::string& filePath)
{
    YAML::Node node;
    std::string fullPathString = RESOURCES_PATH;
    fullPathString += filePath;

    SerializeScene(scene, node);

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

    DeserializeScene(scene, node);

    Log::LogInfo("Loaded scene " + filePath);

    return scene;
}

void SerializationManager::BackupScene(Scene* scene)
{
    if (_sceneBackup != nullptr)
    {
        delete _sceneBackup;
        _sceneBackup = nullptr;
    }

    _sceneBackup = new YAML::Node;
    SerializeScene(scene, *_sceneBackup);
}

void SerializationManager::RestoreScene(Scene* scene)
{
    if (_sceneBackup == nullptr)
    {
        Log::LogError("No scene to restore from backup");
        return;
    }

    DeserializeScene(scene, *_sceneBackup);
}

void SerializationManager::RegisterAttribute(ComponentTypeID typeID, const AttributeInfo& attributeInfo)
{
    _attributesInfo[typeID].emplace_back(attributeInfo);
}

std::vector<AttributeInfo>& SerializationManager::GetAttributes(ComponentTypeID typeID)
{
    return _attributesInfo[typeID];
}

void SerializationManager::SerializeScene(Scene* scene, YAML::Node& node)
{
    SerializationContext context;
    context.SerializedScene = scene;
    context.ResourcesSource = Application::Context()->Resources;

    node["name"] = scene->_name;
    node["mainCamera"] = context.GetUUID(scene->_mainCameraEntity);
    node["childrenCount"] = scene->GetChildrenCount();
    node["firstChildNode"] = context.GetUUID(scene->GetFirstChildNode());

    auto entitiesNode = node["entities"];

    auto activeIterator = scene->entitiesRegistry->GetComponentIterator<IDComponent>();
    auto inactiveIterator = scene->entitiesRegistry->GetComponentIterator<IDComponent>(false);
    int activeCount = activeIterator.Size();
    int entitiesCount = activeIterator.Size() + inactiveIterator.Size();

    for (int i = 0; i < entitiesCount; ++i)
    {
        auto& uuidComponent = i < activeCount ? activeIterator[i] : inactiveIterator[i - activeCount];
        EntityID entityID = uuidComponent.Owner;

        YAML::Node entityNode;
        entityNode["uuid"] = uuidComponent.GetUUID();
        entityNode["state"] = (uint32_t)scene->entitiesRegistry->EntityGetState(entityID);
        YAML::Node componentsNode = entityNode["components"];

        std::vector<RawComponentData> rawData;
        scene->entitiesRegistry->GetAllComponentsForEntity(entityID, rawData);
        for (auto& dataPair : rawData)
        {
            auto object = static_cast<Serializable*>(dataPair.Data);
            auto typeInfo = TypeInfoStorage::GetTypeInfo(dataPair.TypeID);

            YAML::Node componentNode;
            Serialize(typeInfo->ID, object, componentNode, context);

            componentsNode[typeInfo->TypeName] = componentNode;
        }

        entitiesNode.push_back(entityNode);
    }
}

void SerializationManager::DeserializeScene(Scene* scene, YAML::Node& node)
{
    SerializationContext context;
    context.SerializedScene = scene;
    context.ResourcesSource = Application::Context()->Resources;

    auto entitiesNode = node["entities"];
    Log::LogInfo("Entities count loaded: {0}", entitiesNode.size());
    if (entitiesNode)
    {
        // Pre pass to fill UUID to EntityID map (for link attributes)
        for (auto entityNode : entitiesNode)
        {
            auto uuid = entityNode["uuid"].as<UUID_TYPE>();
            if (uuid != NULL_UUID)
            {
                auto entity = scene->entitiesRegistry->CreateNewEntity();
                scene->SetEntityByUUID(uuid, entity);
            }
        }

        // Main pass
        for (auto entityNode : entitiesNode)
        {
            auto uuid = entityNode["uuid"].as<UUID_TYPE>();
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

                    auto object = static_cast<Serializable*>(scene->entitiesRegistry->RestoreComponent(typeID, entity).Data);
                    if (!object)
                    {
                        Log::LogError("Object of type {0} was not restored properly", typeName);
                        continue;
                    }

                    Deserialize(typeID, object, componentNode.second, context);
                }
            }
        }
    }

    // Setting these properties after UUIDs loaded
    scene->_name = node["name"].as<std::string>();
    scene->_mainCameraEntity = context.GetEntityID(node["mainCamera"].as<UUID_TYPE>());
    scene->SetChildrenCount(node["childrenCount"].as<uint32_t>());
    scene->SetFirstChildNode(context.GetEntityID(node["firstChildNode"].as<UUID_TYPE>()));
}

void SerializationManager::Serialize(ComponentTypeID typeID, Serializable* object, YAML::Node& node, SerializationContext& context)
{
    if (_attributesInfo.find(typeID) != _attributesInfo.end())
    {
        for (auto attribute : _attributesInfo[typeID])
        {
            attribute.Serialize(object, node, context);
        }
    }
}

void SerializationManager::Deserialize(ComponentTypeID typeID, Serializable* object, const YAML::Node& node, SerializationContext& context)
{
    if (_attributesInfo.find(typeID) != _attributesInfo.end())
    {
        for (auto attribute : _attributesInfo[typeID])
        {
            attribute.Deserialize(object, node, context);
        }
    }
}
