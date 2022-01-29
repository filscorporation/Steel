#include "MeshRenderer.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/Core/Log.h"

void MeshRenderer::RegisterType()
{
    REGISTER_COMPONENT(MeshRenderer);
    REGISTER_RESOURCE_ATTRIBUTE(MeshRenderer, "material", GetMaterial, SetMaterial, Material*, ResourceTypes::Material, AttributeFlags::Public);
    // For now mesh is fully procedural structure therefor it is not serialized or stored.
    // Maybe later mesh will become resource
}

void MeshRenderer::SetDefault(EntitiesRegistry* entitiesRegistry)
{
    _material = Application::Instance->GetResourcesManager()->DefaultMeshMaterial();
}

void MeshRenderer::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    isDirty = true;
}

void MeshRenderer::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    vb.Clear();
    ib.Clear();
}

void MeshRenderer::Rebuild(Transformation& transformation)
{
    RebuildInner(transformation);
}

void MeshRenderer::Draw(RenderContext* renderContext)
{
    if (isDirty)
        RebuildInner(GetComponentS<Transformation>(Owner));

    if (vb.IsEmpty() || ib.IsEmpty())
        return;

    DrawCall drawCall;
    drawCall.VB = vb;
    drawCall.IB = ib;
    drawCall.RenderMaterial = _material;
    drawCall.CustomProperties = _customProperties;
    drawCall.SortingOrder = _sortingOrder;
    drawCall.Queue = _color.a < 1.0f ? RenderingQueue::Transparent : RenderingQueue::Opaque;

    renderContext->List.AddDrawCall(drawCall);
}

void MeshRenderer::SetMesh(const Mesh& mesh)
{
    _mesh.Clear();
    isDirty = true;

    if (!mesh.Validate())
    {
        Log::LogWarning("Trying to set invalid mesh to mesh renderer {0}", Owner);
        return;
    }

    _mesh = mesh;
}

const Mesh& MeshRenderer::GetMesh()
{
    return _mesh;
}

void MeshRenderer::SetMaterial(Material* material)
{
    _material = material;
    isDirty = true;
}

Material* MeshRenderer::GetMaterial() const
{
    return _material;
}

void MeshRenderer::SetCustomProperties(const MaterialPropertyBlock& properties)
{
    _customProperties = properties;
    isDirty = true;
}

const MaterialPropertyBlock& MeshRenderer::GetCustomProperties()
{
    if (isDirty)
        RebuildInner(GetComponentS<Transformation>(Owner));

    return _customProperties;
}

void MeshRenderer::RebuildInner(Transformation& transformation)
{
    isDirty = false;

    _sortingOrder = transformation.GetGlobalSortingOrderCached();

    _customProperties.UpdateHash();

    ib.Clear();
    vb.Clear();

    if (_material == nullptr || _mesh.IsEmpty())
        return;

    bool hasColors = !_mesh.Colors.empty();
    bool hasUVs = !_mesh.UV.empty();

    uint32_t blockSize = 3;
    if (hasColors) blockSize += 4;
    if (hasUVs) blockSize += 2;

    uint32_t verticesDataSize = _mesh.Vertices.size() * blockSize;

    glm::mat4 matrix = transformation.GetTransformationMatrixCached();
    auto verticesData = new float[verticesDataSize];
    for (int i = 0; i < _mesh.Vertices.size(); ++i)
    {
        uint32_t offset = i * blockSize;
        glm::vec3 vertex = matrix * glm::vec4(_mesh.Vertices[i], 1.0f);
        verticesData[offset++] = vertex.x;
        verticesData[offset++] = vertex.y;
        verticesData[offset++] = vertex.z;
        if (hasColors)
        {
            verticesData[offset++] = _mesh.Colors[i].r;
            verticesData[offset++] = _mesh.Colors[i].g;
            verticesData[offset++] = _mesh.Colors[i].b;
            verticesData[offset++] = _mesh.Colors[i].a;
        }
        if (hasUVs)
        {
            verticesData[offset++] = _mesh.UV[i].x;
            verticesData[offset++] = _mesh.UV[i].y;
        }
    }

    std::vector<VertexAttribute> attributes;
    uint32_t offset = 0;
    attributes.emplace_back(offset++, 3);
    if (hasUVs)
        attributes.emplace_back(offset++, 2);
    if (hasColors)
        attributes.emplace_back(offset++, 4);

    auto indicesData = new uint32_t[_mesh.Indices.size()];
    for (int i = 0; i < _mesh.Indices.size(); ++i)
    {
        indicesData[i] = _mesh.Indices[i];
    }

    vb.Create(verticesData, verticesDataSize, attributes);
    ib.Create(indicesData, _mesh.Indices.size());
}
