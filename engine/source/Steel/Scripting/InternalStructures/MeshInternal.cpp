#include "MeshInternal.h"
#include "Steel/Scripting/ScriptingCore.h"

void MeshInternal::FromMesh(const Mesh& mesh)
{
    Vertices = ScriptingCore::ToMonoSimpleTypeArray(mesh.Vertices, SimpleAPITypes::Vector3);
    Colors = ScriptingCore::ToMonoSimpleTypeArray(mesh.Colors, SimpleAPITypes::Color);
    UV = ScriptingCore::ToMonoSimpleTypeArray(mesh.UV, SimpleAPITypes::Vector2);
    Indices = ScriptingCore::ToMonoIntArray(mesh.Indices);
}

void MeshInternal::ToMesh(Mesh& outMesh) const
{
    ScriptingCore::FromMonoSimpleTypeArray(Vertices, outMesh.Vertices, SimpleAPITypes::Vector3);
    if (Colors != nullptr)
        ScriptingCore::FromMonoSimpleTypeArray(Colors, outMesh.Colors, SimpleAPITypes::Color);
    if (UV != nullptr)
        ScriptingCore::FromMonoSimpleTypeArray(UV, outMesh.UV, SimpleAPITypes::Vector2);
    ScriptingCore::FromMonoIntArray(Indices, outMesh.Indices);
}
