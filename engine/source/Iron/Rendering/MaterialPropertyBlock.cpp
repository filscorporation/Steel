#include "MaterialPropertyBlock.h"
#include "OpenGLAPI.h"

void MaterialPropertyBlock::Apply(Shader* shader) const
{
    for (auto& property : floatProperties)
        OpenGLAPI::SetUniformFloat(shader->GetUniformLocation(property.first), property.second);
    for (auto& property : intProperties)
        OpenGLAPI::SetUniformInt(shader->GetUniformLocation(property.first), property.second);
    for (auto& property : uintProperties)
        OpenGLAPI::SetUniformUInt(shader->GetUniformLocation(property.first), property.second);
    for (auto& property : textureProperties)
        OpenGLAPI::BindTexture(property.second, property.first);
    for (auto& property : mat4Properties)
        OpenGLAPI::SetUniformMat4F(shader->GetUniformLocation(property.first), property.second);
}

template<class T>
inline void HashCombine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<class K, class V>
std::size_t MapHash(const std::map<K, V>& value)
{
    std::size_t seed = value.size();
    for(auto& pair : value)
    {
        HashCombine(seed, pair.first);
        HashCombine(seed, pair.second);
    }
    return seed;
}

std::size_t MaterialPropertyBlock::Hash(const MaterialPropertyBlock& value)
{
    std::size_t seed = 0;
    HashCombine(seed, MapHash(value.floatProperties));
    HashCombine(seed, MapHash(value.intProperties));
    HashCombine(seed, MapHash(value.uintProperties));
    HashCombine(seed, MapHash(value.textureProperties));
    HashCombine(seed, MapHash(value.mat4Properties));

    return seed;
}

void MaterialPropertyBlock::UpdateHash()
{
    hash = Hash(*this);
}

size_t MaterialPropertyBlock::GetHash() const
{
    return hash;
}

void MaterialPropertyBlock::SetFloat(const std::string& name, float value)
{
    floatProperties[name] = value;
    UpdateHash();
}

void MaterialPropertyBlock::SetInt(const std::string& name, int value)
{
    intProperties[name] = value;
    UpdateHash();
}

void MaterialPropertyBlock::SetUInt(const std::string& name, uint32_t value)
{
    uintProperties[name] = value;
    UpdateHash();
}

void MaterialPropertyBlock::SetTexture(uint32_t textureSlot, uint32_t textureID)
{
    textureProperties[textureSlot] = textureID;
    UpdateHash();
}

void MaterialPropertyBlock::SetMat4(const std::string& name, const float* value)
{
    mat4Properties[name] = value;
    UpdateHash();
}
