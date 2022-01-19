#include "MeshInternal.h"
#include "Steel/Scripting/ScriptingCore.h"

void MeshInternal::FromMesh(const Mesh& mesh)
{
    Vertices = ScriptingCore::ToMonoSimpleTypeArray(mesh.Vertices, SimpleAPITypes::Vector3);
    Colors = ScriptingCore::ToMonoSimpleTypeArray(mesh.Colors, SimpleAPITypes::Vector2);
    UV = ScriptingCore::ToMonoSimpleTypeArray(mesh.UV, SimpleAPITypes::Color);
    Indices = ScriptingCore::ToMonoIntArray(mesh.Indices);
}

void MeshInternal::ToMesh(Mesh& outMesh) const
{
    ScriptingCore::FromMonoSimpleTypeArray(Vertices, outMesh.Vertices, SimpleAPITypes::Vector3);
    if (Colors != nullptr)
        ScriptingCore::FromMonoSimpleTypeArray(Colors, outMesh.Colors, SimpleAPITypes::Vector2);
    if (UV != nullptr)
        ScriptingCore::FromMonoSimpleTypeArray(UV, outMesh.UV, SimpleAPITypes::Color);
    ScriptingCore::FromMonoIntArray(Indices, outMesh.Indices);
}
