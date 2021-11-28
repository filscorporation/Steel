#include <glm/gtc/type_ptr.hpp>

#include "MaterialPropertyBlock.h"
#include "Steel/Rendering/Core/OpenGLAPI.h"
#include "Steel/Core/Log.h"

void MaterialPropertyBlock::Apply(Shader* shader) const
{
    for (auto& property : floatProperties)
        OpenGLAPI::SetUniformFloat(shader->GetUniformLocation(property.first), property.second);
    for (auto& property : intProperties)
        OpenGLAPI::SetUniformInt(shader->GetUniformLocation(property.first), property.second);
    for (auto& property : textureProperties)
    {
        int uniformID = shader->GetUniformLocation(property.first);
        int textureSlot = shader->GetTextureSlot(uniformID);
        OpenGLAPI::BindTexture(property.second, textureSlot);
        OpenGLAPI::SetUniformInt(uniformID, textureSlot);
    }
    for (auto& property : colorProperties)
        OpenGLAPI::SetUniformVec4F(shader->GetUniformLocation(property.first), glm::value_ptr(property.second));
    for (auto& property : mat4Properties)
        OpenGLAPI::SetUniformMat4F(shader->GetUniformLocation(property.first), property.second);

    OpenGLAPI::SetDepthMask(depthMask);
    OpenGLAPI::SetDepthFunc(depthFunc);
    OpenGLAPI::SetStencilMask(stencilMask);
    OpenGLAPI::SetStencilFunc(stencilFunction, stencilFunctionRef, stencilFunctionMask);
    OpenGLAPI::SetStencilOperation(stencilFailOperation, stencilZFailOperation, stencilZPassOperation);
}

template<class T>
inline void HashCombine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

inline void HashCombine(std::size_t& seed, const glm::vec4& v)
{
    HashCombine(seed, v[0]);
    HashCombine(seed, v[1]);
    HashCombine(seed, v[2]);
    HashCombine(seed, v[3]);
}

inline void HashCombineMat4(std::size_t& seed, const float* v)
{
    for (int i = 0; i < 16; ++i)
        HashCombine(seed, v[i]);
}

template<class K, class V>
std::size_t MapHash(const std::map<K, V>& value)
{
    std::size_t seed = value.size();
    for (auto& pair : value)
    {
        HashCombine(seed, pair.first);
        HashCombine(seed, pair.second);
    }
    return seed;
}

template<class K>
std::size_t MapHashMat4(const std::map<K, const float*>& value)
{
    std::size_t seed = value.size();
    for (auto& pair : value)
    {
        HashCombine(seed, pair.first);
        HashCombineMat4(seed, pair.second);
    }
    return seed;
}

std::size_t MaterialPropertyBlock::Hash(const MaterialPropertyBlock& value)
{
    std::size_t seed = 0;
    HashCombine(seed, MapHash(value.floatProperties));
    HashCombine(seed, MapHash(value.intProperties));
    HashCombine(seed, MapHash(value.textureProperties));
    HashCombine(seed, MapHash(value.colorProperties));
    HashCombine(seed, MapHashMat4(value.mat4Properties));
    HashCombine(seed, value.depthMask);
    HashCombine(seed, value.depthFunc);
    HashCombine(seed, value.stencilMask);
    HashCombine(seed, value.stencilFunction);
    HashCombine(seed, value.stencilFunctionRef);
    HashCombine(seed, value.stencilFunctionMask);
    HashCombine(seed, value.stencilFailOperation);
    HashCombine(seed, value.stencilZFailOperation);
    HashCombine(seed, value.stencilZPassOperation);

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

void MaterialPropertyBlock::SetTexture(const std::string& name, uint32_t textureID)
{
    textureProperties[name] = textureID;
    UpdateHash();
}

void MaterialPropertyBlock::SetColor(const std::string& name, const glm::vec4& value)
{
    colorProperties[name] = value;
    UpdateHash();
}

void MaterialPropertyBlock::SetMat4(const std::string& name, const float* value)
{
    mat4Properties[name] = value;
    UpdateHash();
}

void MaterialPropertyBlock::SetDepthMask(bool mask)
{
    depthMask = mask;
    UpdateHash();
}

void MaterialPropertyBlock::SetDepthFunc(ComparisonFunctions::ComparisonFunction func)
{
    depthFunc = func;
    UpdateHash();
}

void MaterialPropertyBlock::SetStencilFunc(ComparisonFunctions::ComparisonFunction func, short ref, short mask)
{
    stencilFunction = func;
    stencilFunctionRef = ref;
    stencilFunctionMask = mask;
    UpdateHash();
}

void MaterialPropertyBlock::SetStencilMask(short mask)
{
    stencilMask = mask;
    UpdateHash();
}

void MaterialPropertyBlock::SetStencilOperation(StencilOperations::StencilOperation fail, StencilOperations::StencilOperation zfail, StencilOperations::StencilOperation zpass)
{
    stencilFailOperation = fail;
    stencilZFailOperation = zfail;
    stencilZPassOperation = zpass;
    UpdateHash();
}
