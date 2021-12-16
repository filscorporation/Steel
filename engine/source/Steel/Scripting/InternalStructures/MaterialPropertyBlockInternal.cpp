#include "MaterialPropertyBlockInternal.h"
#include "Steel/Scripting/ScriptingCore.h"

void MaterialPropertyBlockInternal::FromMaterialPropertyBlock(const MaterialPropertyBlock& properties)
{
    std::vector<std::string> floatKeys;
    std::vector<float> floatValues;
    floatKeys.reserve(properties.floatProperties.size());
    floatValues.reserve(properties.floatProperties.size());
    for (auto& pair : properties.floatProperties)
    {
        floatKeys.emplace_back(pair.first);
        floatValues.emplace_back(pair.second);
    }
    floatPropertiesKeys = ScriptingCore::ToMonoStringArray(floatKeys);
    floatPropertiesValues = ScriptingCore::ToMonoFloatArray(floatValues);

    std::vector<std::string> intKeys;
    std::vector<int> intValues;
    intKeys.reserve(properties.intProperties.size());
    intValues.reserve(properties.intProperties.size());
    for (auto& pair : properties.intProperties)
    {
        intKeys.emplace_back(pair.first);
        intValues.emplace_back(pair.second);
    }
    intPropertiesKeys = ScriptingCore::ToMonoStringArray(intKeys);
    intPropertiesValues = ScriptingCore::ToMonoIntArray(intValues);

    std::vector<std::string> textureKeys;
    std::vector<uint32_t> textureValues;
    textureKeys.reserve(properties.textureProperties.size());
    textureValues.reserve(properties.textureProperties.size());
    for (auto& pair : properties.textureProperties)
    {
        textureKeys.emplace_back(pair.first);
        textureValues.emplace_back(pair.second);
    }
    texturePropertiesKeys = ScriptingCore::ToMonoStringArray(textureKeys);
    texturePropertiesValues = ScriptingCore::ToMonoUInt32Array(textureValues);

    std::vector<std::string> colorKeys;
    std::vector<glm::vec4> colorValues;
    colorKeys.reserve(properties.colorProperties.size());
    colorValues.reserve(properties.colorProperties.size());
    for (auto& pair : properties.colorProperties)
    {
        colorKeys.emplace_back(pair.first);
        colorValues.emplace_back(pair.second);
    }
    colorPropertiesKeys = ScriptingCore::ToMonoStringArray(colorKeys);
    colorPropertiesValues = ScriptingCore::ToMonoDataTypeArray(colorValues, 1);

    depthMask = properties.depthMask;
    depthFunction = properties.depthFunc;
    stencilMask = properties.stencilMask;
    stencilFunction = properties.stencilFunction;
    stencilFunctionRef = properties.stencilFunctionRef;
    stencilFunctionMask = properties.stencilFunctionMask;
    stencilFailOperation = properties.stencilFailOperation;
    stencilZFailOperation = properties.stencilZFailOperation;
    stencilZPassOperation = properties.stencilZPassOperation;
}

void MaterialPropertyBlockInternal::ToMaterialPropertyBlock(MaterialPropertyBlock& outProperties) const
{
    std::vector<std::string> floatKeys;
    std::vector<float> floatValues;
    ScriptingCore::FromMonoStringArray(floatPropertiesKeys, floatKeys);
    ScriptingCore::FromMonoFloatArray(floatPropertiesValues, floatValues);
    for (int i = 0; i < floatKeys.size(); ++i)
        outProperties.SetFloat(floatKeys[i], floatValues[i]);

    std::vector<std::string> intKeys;
    std::vector<int> intValues;
    ScriptingCore::FromMonoStringArray(intPropertiesKeys, intKeys);
    ScriptingCore::FromMonoIntArray(intPropertiesValues, intValues);
    for (int i = 0; i < intKeys.size(); ++i)
        outProperties.SetInt(intKeys[i], intValues[i]);

    std::vector<std::string> textureKeys;
    std::vector<uint32_t> textureValues;
    ScriptingCore::FromMonoStringArray(texturePropertiesKeys, textureKeys);
    ScriptingCore::FromMonoUInt32Array(texturePropertiesValues, textureValues);
    for (uint32_t i = 0; i < textureKeys.size(); ++i)
        outProperties.SetTexture(textureKeys[i], textureValues[i]);

    std::vector<std::string> colorKeys;
    std::vector<glm::vec4> colorValues;
    ScriptingCore::FromMonoStringArray(colorPropertiesKeys, colorKeys);
    ScriptingCore::FromMonoDataTypeArray(colorPropertiesValues, colorValues, 1);
    for (uint32_t i = 0; i < colorKeys.size(); ++i)
        outProperties.SetColor(colorKeys[i], colorValues[i]);

    outProperties.depthMask = depthMask;
    outProperties.depthFunc = depthFunction;
    outProperties.stencilMask = stencilMask;
    outProperties.stencilFunction = stencilFunction;
    outProperties.stencilFunctionRef = stencilFunctionRef;
    outProperties.stencilFunctionMask = stencilFunctionMask;
    outProperties.stencilFailOperation = stencilFailOperation;
    outProperties.stencilZFailOperation = stencilZFailOperation;
    outProperties.stencilZPassOperation = stencilZPassOperation;
}
