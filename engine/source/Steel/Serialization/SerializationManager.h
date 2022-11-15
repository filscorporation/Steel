#pragma once

#include "AttributeInfo.h"
#include "SerializationConvert.h"
#include "Steel/EntityComponentSystem/EntitiesRegistry.h"
#include "Steel/Resources/SceneData.h"
#include "Steel/Scene/SceneBackup.h"

#include <typeindex>
#include <unordered_map>
#include <vector>

class Scene;

class SerializationManager
{
public:
    static void Init();
    static void Terminate();

    static void SerializeScene(Scene* scene, const std::string& filePath);
    static void DeserializeScene(Scene* scene, const std::string& filePath);
    static SceneBackup* BackupScene(Scene* scene, SceneData* sceneData);
    static void RestoreScene(SceneBackup* backup, Scene* scene);

    static void SerializeScene(Scene* scene, YAML::Node& node);
    static void DeserializeScene(Scene* scene, YAML::Node& node);

    static void Serialize(ComponentTypeID typeID, Serializable* object, YAML::Node& node, SerializationContext& context);
    static void Deserialize(ComponentTypeID typeID, Serializable* object, const YAML::Node& node, SerializationContext& context);

    static void SerializeResource(Resource* resource, const std::string& filePath);
    static void DeserializeResource(Resource* resource, const std::string& filePath);

    static void RegisterAttribute(ComponentTypeID typeID, const AttributeInfo& attributeInfo);
    static std::vector<AttributeInfo>& GetAttributes(ComponentTypeID typeID);
    template<class T>
    static std::vector<AttributeInfo>& GetAttributes() { return GetAttributes(TYPE_ID(T)); }

private:
    static std::unordered_map<ComponentTypeID, std::vector<AttributeInfo>> _attributesInfo;
};
