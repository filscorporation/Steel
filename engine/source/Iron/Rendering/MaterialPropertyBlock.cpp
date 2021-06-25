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

void MaterialPropertyBlock::SetFloat(const std::string& name, float value)
{
    floatProperties[name] = value;
}

void MaterialPropertyBlock::SetInt(const std::string& name, int value)
{
    intProperties[name] = value;
}

void MaterialPropertyBlock::SetUInt(const std::string& name, uint32_t value)
{
    uintProperties[name] = value;
}

void MaterialPropertyBlock::SetTexture(uint32_t textureSlot, uint32_t textureID)
{
    textureProperties[textureSlot] = textureID;
}

void MaterialPropertyBlock::SetMat4(const std::string& name, const float* value)
{
    mat4Properties[name] = value;
}
