#pragma once

#include <glm/glm.hpp>
#include <string>
#include <map>

#include "Shader.h"

#define MAIN_TEX "main_tex"

struct MaterialPropertyBlock
{
public:
    void Apply(Shader* shader) const;
    void UpdateHash();
    size_t GetHash() const;

    void SetFloat(const std::string& name, float value);
    void SetInt(const std::string& name, int value);
    void SetTexture(const std::string& name, uint32_t textureID);
    void SetColor(const std::string& name, const glm::vec4& value);

    void SetMat4(const std::string& name, const float* value);

private:
    static std::size_t Hash(const MaterialPropertyBlock& value);

    size_t hash;

    std::map<std::string, float> floatProperties;
    std::map<std::string, int> intProperties;
    std::map<std::string, uint32_t> textureProperties;
    std::map<std::string, glm::vec4> colorProperties;

    std::map<std::string, const float*> mat4Properties;

    friend class MaterialPropertyBlockInternal;
};
