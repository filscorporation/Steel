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

    short stencilMask;
    StencilFunctions::StencilFunction stencilFunction;
    short stencilFunctionRef;
    short stencilFunctionMask;
    StencilOperations::StencilOperation stencilFailOperation;
    StencilOperations::StencilOperation stencilZFailOperation;
    StencilOperations::StencilOperation stencilZPassOperation;

    void FromMaterialPropertyBlock(const MaterialPropertyBlock& properties);
    void ToMaterialPropertyBlock(MaterialPropertyBlock& outProperties) const;
};
