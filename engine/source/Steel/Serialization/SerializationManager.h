#pragma once

#include "AttributeInfo.h"
#include "SerializationConvert.h"
#include "../EntityComponentSystem/EntitiesRegistry.h"

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
    static Scene* DeserializeScene(const std::string& filePath);

    static void RegisterAttribute(ComponentTypeID classTypeID, const AttributeInfo& attributeInfo);
    static std::vector<AttributeInfo>& GetAttributes(ComponentTypeID classTypeID);
    template<class T>
    static std::vector<AttributeInfo>& GetAttributes() { return GetAttributes(TYPE_ID(T)); }

private:
    static bool SerializeScene(Scene* scene, YAML::Node& node);
    static bool DeserializeScene(Scene* scene, YAML::Node& node);
    static void RegisterComponents();

    static std::unordered_map<ComponentTypeID, std::vector<AttributeInfo>> _attributesInfo;
};
