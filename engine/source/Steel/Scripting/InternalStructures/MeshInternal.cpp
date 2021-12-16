#include "MeshInternal.h"
#include "Steel/Scripting/ScriptingCore.h"

void MeshInternal::FromMesh(const Mesh& mesh)
{
    Vertices = ScriptingCore::ToMonoDataTypeArray(mesh.Vertices, 3);
    Colors = ScriptingCore::ToMonoDataTypeArray(mesh.Colors, 1);
    UV = ScriptingCore::ToMonoDataTypeArray(mesh.UV, 2);
    Indices = ScriptingCore::ToMonoIntArray(mesh.Indices);
}

void MeshInternal::ToMesh(Mesh& outMesh) const
{
    ScriptingCore::FromMonoDataTypeArray(Vertices, outMesh.Vertices, 2);
    if (Colors != nullptr)
        ScriptingCore::FromMonoDataTypeArray(Colors, outMesh.Colors, 1);
    if (UV != nullptr)
        ScriptingCore::FromMonoDataTypeArray(UV, outMesh.UV, 3);
    ScriptingCore::FromMonoIntArray(Indices, outMesh.Indices);
}
