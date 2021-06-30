#pragma once

#include <mono/jit/jit.h>

#include "../Rendering/MaterialPropertyBlock.h"

struct MaterialPropertyBlockInternal
{
public:
    MonoArray* floatPropertiesKeys;
    MonoArray* floatPropertiesValues;
    MonoArray* intPropertiesKeys;
    MonoArray* intPropertiesValues;
    MonoArray* texturePropertiesKeys;
    MonoArray* texturePropertiesValues;
    MonoArray* colorPropertiesKeys;
    MonoArray* colorPropertiesValues;

    void FromMaterialPropertyBlock(const MaterialPropertyBlock& properties);
    void ToMaterialPropertyBlock(MaterialPropertyBlock& outProperties) const;
};
