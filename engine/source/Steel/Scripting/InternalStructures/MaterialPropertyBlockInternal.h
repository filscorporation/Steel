#pragma once

#include "mono/include/mono/jit/jit.h"

#include "Steel/Rendering/MaterialSystem/MaterialPropertyBlock.h"

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

    bool depthMask;
    ComparisonFunctions::ComparisonFunction depthFunction;
    short stencilMask;
    ComparisonFunctions::ComparisonFunction stencilFunction;
    short stencilFunctionRef;
    short stencilFunctionMask;
    StencilOperations::StencilOperation stencilFailOperation;
    StencilOperations::StencilOperation stencilZFailOperation;
    StencilOperations::StencilOperation stencilZPassOperation;

    void FromMaterialPropertyBlock(const MaterialPropertyBlock& properties);
    void ToMaterialPropertyBlock(MaterialPropertyBlock& outProperties) const;
};
