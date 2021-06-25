#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include "Shader.h"

struct MaterialPropertyBlock
{
public:
    void Apply(Shader* shader) const;

    void SetFloat(const std::string& name, float value);
    void SetInt(const std::string& name, int value);
    void SetUInt(const std::string& name, uint32_t value);
    void SetTexture(uint32_t textureSlot, uint32_t textureID);
    void SetMat4(const std::string& name, const float* value);
private:
    std::unordered_map<std::string, float> floatProperties;
    std::unordered_map<std::string, int> intProperties;
    std::unordered_map<std::string, uint32_t> uintProperties;
    std::unordered_map<uint32_t, uint32_t> textureProperties;
    std::unordered_map<std::string, const float*> mat4Properties;
};
