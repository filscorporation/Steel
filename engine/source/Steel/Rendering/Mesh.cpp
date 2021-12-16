#include "Mesh.h"

void Mesh::Clear()
{
    Vertices.clear();
    Colors.clear();
    UV.clear();
    Indices.clear();
}

bool Mesh::IsEmpty() const
{
    return Vertices.empty() || Indices.empty();
}

bool Mesh::Validate() const
{
    if (!Colors.empty() && Colors.size() != Vertices.size()
        || !UV.empty() && UV.size() != Vertices.size())
        return false;

    for (int index : Indices)
    {
        if (index >= Vertices.size())
            return false;
    }

    return true;
}
