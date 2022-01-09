#include "SpriteRenderer.h"
#include "Steel/Scene/SceneHelper.h"

void SpriteRenderer::RegisterType()
{
    REGISTER_TYPE(SpriteRenderer);
    REGISTER_RESOURCE_ATTRIBUTE(SpriteRenderer, "material", GetMaterial, SetMaterial, Material*, ResourceTypes::Material, AttributeFlags::Public);
    REGISTER_RESOURCE_ATTRIBUTE(SpriteRenderer, "image", GetImage, SetImage, Sprite*, ResourceTypes::Sprite, AttributeFlags::Public);
}

void SpriteRenderer::OnCopied()
{
    isDirty = true;
    vb.Refresh();
    ib.Refresh();
}

void SpriteRenderer::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    isDirty = true;
}

void SpriteRenderer::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    vb.Clear();
    ib.Clear();
}

void SpriteRenderer::Rebuild(Transformation& transformation)
{
    RebuildInner(transformation);
}

void SpriteRenderer::Draw(RenderContext* renderContext)
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
    drawCall.Queue = _image->IsTransparent ? RenderingQueue::Transparent : RenderingQueue::Opaque;

    renderContext->List.AddDrawCall(drawCall);
}

void SpriteRenderer::SetMaterial(Material* material)
{
    _material = material;
    isDirty = true;
}

Material* SpriteRenderer::GetMaterial() const
{
    return _material;
}

void SpriteRenderer::SetCustomProperties(const MaterialPropertyBlock& properties)
{
    _customProperties = properties;
    isDirty = true;
}

const MaterialPropertyBlock& SpriteRenderer::GetCustomProperties()
{
    if (isDirty)
        RebuildInner(GetComponentS<Transformation>(Owner));

    return _customProperties;
}

void SpriteRenderer::SetImage(Sprite* image)
{
    _image = image;
    isDirty = true;
}

Sprite* SpriteRenderer::GetImage() const
{
    return _image;
}

void SpriteRenderer::SetImageTileIndex(uint32_t index)
{
    if (index == currentImageTileIndex)
        return;

    currentImageTileIndex = index;
    isDirty = true;
}

uint32_t SpriteRenderer::GetImageTileIndex() const
{
    return currentImageTileIndex;
}

glm::vec2 SpriteRenderer::GetWorldSize()
{
    if (_image == nullptr)
        return { 0, 0 };

    return GetComponentS<Transformation>(Owner).GetScale() * _image->GetRealWorldSize();
}

void SpriteRenderer::RebuildInner(Transformation& transformation)
{
    isDirty = false;

    _sortingOrder = transformation.GetGlobalSortingOrderCached();

    _customProperties.SetTexture(MAIN_TEX, _image == nullptr ? 0 : _image->SpriteTexture->GetTextureID());
    _customProperties.UpdateHash();

    if (_image != nullptr && _material == nullptr)
        _material = Application::Instance->GetResourcesManager()->DefaultSpriteMaterial();

    ib.Clear();
    vb.Clear();

    if (_image != nullptr && _image->SpriteTexture != nullptr)
    {
        glm::vec2 texCoords[4];
        if (_image->IsSpriteSheet)
        {
            _image->GetTexCoord(currentImageTileIndex, texCoords);
        }
        else
        {
            texCoords[0] = glm::vec2(1.0f, 0.0f);
            texCoords[1] = glm::vec2(1.0f, 1.0f);
            texCoords[2] = glm::vec2(0.0f, 0.0f);
            texCoords[3] = glm::vec2(0.0f, 1.0f);
        }

        glm::vec4 defaultVertices[4];
        defaultVertices[0] = glm::vec4(1.0f - _image->Pivot.x, 1.0f - _image->Pivot.y, 0.0f, 1.0f);
        defaultVertices[1] = glm::vec4(1.0f - _image->Pivot.x, 0.0f - _image->Pivot.y, 0.0f, 1.0f);
        defaultVertices[2] = glm::vec4(0.0f - _image->Pivot.x, 1.0f - _image->Pivot.y, 0.0f, 1.0f);
        defaultVertices[3] = glm::vec4(0.0f - _image->Pivot.x, 0.0f - _image->Pivot.y, 0.0f, 1.0f);

        glm::vec3 vertices[4];
        glm::mat4 matrix = transformation.GetTransformationMatrixCached()
                           * glm::scale(glm::mat4(1.0f), _image->GetRealWorldSize());
        for (int i = 0; i < 4; ++i)
            vertices[i] = matrix * defaultVertices[i];

        auto indices = new uint32_t[6]{ 0, 1, 2, 1, 2, 3 };

        vb.Create(vertices, _color, texCoords);
        ib.Create(indices, 6);
    }
}
