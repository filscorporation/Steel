#pragma once

#include "mono/include/mono/jit/jit.h"

#include "Steel/Rendering/Mesh.h"

struct MeshInternal
{
public:
    MonoArray* Vertices;
    MonoArray* Colors;
    MonoArray* UV;
    MonoArray* Indices;

    void FromMesh(const Mesh& mesh);
    void ToMesh(Mesh& outMesh) const;
};
