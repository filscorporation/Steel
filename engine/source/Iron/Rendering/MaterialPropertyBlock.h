#pragma once

#include <glm/glm.hpp>
#include <string>
#include <map>

#include "Shader.h"

struct MaterialPropertyBlock
{
public:
    void Apply(Shader* shader) const;
    void UpdateHash();
    size_t GetHash() const;

    void SetFloat(const std::string& name, float value);
    void SetInt(const std::string& name, int value);
    void SetUInt(const std::string& name, uint32_t value);
    void SetTexture(uint32_t textureSlot, uint32_t textureID);
    void SetMat4(const std::string& name, const float* value);

private:
    static std::size_t Hash(const MaterialPropertyBlock& value);

    size_t hash;

    std::map<std::string, float> floatProperties;
    std::map<std::string, int> intProperties;
    std::map<std::string, uint32_t> uintProperties;
    std::map<uint32_t, uint32_t> textureProperties;
    std::map<std::string, const float*> mat4Properties;
};
