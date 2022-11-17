#include "UIImage.h"
#include "Steel/Rendering/Screen.h"
#include "Steel/Scene/Hierarchy.h"
#include "Steel/Scene/SceneHelper.h"
#include "Steel/UI/UIEventHandler.h"
#include "Steel/Serialization/AttributesRegistration.h"

void UIImage::RegisterType()
{
    REGISTER_COMPONENT(UIImage);
    REGISTER_RESOURCE_ATTRIBUTE(UIImage, "material", GetMaterial, SetMaterial, Material*, ResourceTypes::Material, AttributeFlags::Public, "Material");
    REGISTER_RESOURCE_ATTRIBUTE(UIImage, "image", GetImage, SetImage, Sprite*, ResourceTypes::Sprite, AttributeFlags::Public, "Image");
    REGISTER_ATTRIBUTE(UIImage, "color", GetColor, SetColor, glm::vec4, AttributeFlags::Public, "Color");
    REGISTER_ATTRIBUTE(UIImage, "consumeEvents", GetConsumeEvents, SetConsumeEvents, bool, AttributeFlags::Public, "Consume events");
}

bool UIImage::Validate(EntitiesRegistry* entitiesRegistry)
{
    return Component::Validate(entitiesRegistry) && CheckRectTransformation(entitiesRegistry);
}

void UIImage::SetDefault(EntitiesRegistry* entitiesRegistry)
{
    _material = Application::Instance->GetResourcesManager()->DefaultUIMaterial();
}

void UIImage::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    initialized = true;

    _clippingLevel = GetClippingLevelUpwards(entitiesRegistry, Owner);

    entitiesRegistry->AddComponent<UIEventHandler>(Owner).Type =
            _consumeEvents ? EventHandlerTypes::Normal : EventHandlerTypes::Transparent;
}

void UIImage::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    vb.Clear();
    ib.Clear();
}

void UIImage::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    isDirty = true;
}

void UIImage::Rebuild(RectTransformation& transformation, bool transformationDirty, bool sortingOrderDirty)
{
    RebuildInner(transformation);
}

void UIImage::Draw(RenderContext* renderContext)
{
    if (isDirty)
        RebuildInner(GetComponentS<RectTransformation>(Owner));

    if (vb.IsEmpty() || ib.IsEmpty())
        return;

    DrawCall drawCall;
    drawCall.VB = vb;
    drawCall.IB = ib;
    drawCall.RenderMaterial = _material;
    drawCall.CustomProperties = _customProperties;
    drawCall.SortingOrder = _sortingOrder;
    drawCall.Queue = _image->GetIsTransparent() ? RenderingQueue::Transparent : RenderingQueue::Opaque;

    renderContext->List.AddDrawCall(drawCall);
}

void UIImage::SetMaterial(Material* material)
{
    _material = material;
    isDirty = true;
}

Material* UIImage::GetMaterial() const
{
    return _material;
}

void UIImage::SetCustomProperties(const MaterialPropertyBlock& properties)
{
    _customProperties = properties;
}

const MaterialPropertyBlock& UIImage::GetCustomProperties()
{
    if (isDirty)
        RebuildInner(GetComponentS<RectTransformation>(Owner));

    return _customProperties;
}

void UIImage::SetImage(Sprite* image)
{
    _image = image;
    isDirty = true;
}

Sprite* UIImage::GetImage() const
{
    return _image;
}

void UIImage::SetColor(const glm::vec4& color)
{
    _color = color;
    isDirty = true;
}

const glm::vec4& UIImage::GetColor() const
{
    return _color;
}

void UIImage::SetImageTileIndex(uint32_t index)
{
    if (index == currentImageTileIndex)
        return;

    currentImageTileIndex = index;
    isDirty = true;
}

uint32_t UIImage::GetImageTileIndex() const
{
    return currentImageTileIndex;
}

void UIImage::SetConsumeEvents(bool consume)
{
    if (initialized && HasComponentS<UIEventHandler>(Owner))
        GetComponentS<UIEventHandler>(Owner).Type
            = consume ? EventHandlerTypes::Normal : EventHandlerTypes::Transparent;
    _consumeEvents = consume;
}

bool UIImage::GetConsumeEvents() const
{
    return _consumeEvents;
}

void UIImage::SetClippingLevel(short clippingLevel)
{
    _clippingLevel = clippingLevel;
    isDirty = true;
}

void UIImage::RebuildInner(RectTransformation& transformation)
{
    isDirty = false;

    _sortingOrder = transformation.GetSortingOrder();

    ib.Clear();
    vb.Clear();

    _customProperties.SetTexture(MAIN_TEX, _image == nullptr ? 0 : _image->GetSpriteTexture()->GetTextureID());
    _customProperties.SetStencilFunc(ComparisonFunctions::Equal, _clippingLevel, 255);
    _customProperties.UpdateHash();

    if (_material == nullptr)
        return;

    glm::mat4 matrix = transformation.GetTransformationMatrixCached();

    if (_image != nullptr && _image->GetSpriteTexture() != nullptr)
    {
        if (_image->GetIsSliced())
        {
            float xtc[4] = {0.0f, (float)_image->GetSliceLeftOffset() / (float)_image->GetSpriteTexture()->GetWidth(),
                             1.0f - (float)_image->GetSliceRightOffset() / (float)_image->GetSpriteTexture()->GetWidth(), 1.0f };
            float ytc[4] = {0.0f, (float)_image->GetSliceBottomOffset() / (float)_image->GetSpriteTexture()->GetHeight(),
                             1.0f - (float)_image->GetSliceTopOffset() / (float)_image->GetSpriteTexture()->GetHeight(), 1.0f };

            glm::vec2 size = transformation.GetRealSizeCached();
            float k = (float)_image->GetPixelsPerUnit() / (float)UILayer::Current()->PixelsPerUnit;
            float xvs[4] = { 0.0f, (float)_image->GetSliceLeftOffset() * k / size.x,
                             1.0f - (float)_image->GetSliceRightOffset() * k / size.x, 1.0f };
            float yvs[4] = { 0.0f, (float)_image->GetSliceBottomOffset() * k / size.y,
                             1.0f - (float)_image->GetSliceTopOffset() * k / size.y, 1.0f };

            const uint32_t indicesSize = 6 * 9;
            auto indices = new uint32_t[indicesSize];
            const uint32_t verticesSize = 9 * 16;
            auto vertices = new float[verticesSize];
            int offset = 0;
            // Nine slice triangles indices (18 triangles for 9 quads)
            for (int j = 0; j < 3; ++j)
            {
                for (int i = 0; i < 3; ++i)
                {
                    indices[offset++] = j * 4 + 0 + i;
                    indices[offset++] = j * 4 + 1 + i;
                    indices[offset++] = j * 4 + 4 + i;
                    indices[offset++] = j * 4 + 1 + i;
                    indices[offset++] = j * 4 + 4 + i;
                    indices[offset++] = j * 4 + 5 + i;
                }
            }

            offset = 0;
            // Nine slice triangles vertices (16 in total)
            for (int j = 0; j < 4; ++j)
            {
                for (int i = 0; i < 4; ++i)
                {
                    glm::vec3 vertex = matrix * glm::vec4(xvs[i] - 0.5f, yvs[j] - 0.5f, 0.0f, 1.0f);
                    vertices[offset++] = vertex[0];
                    vertices[offset++] = vertex[1];
                    vertices[offset++] = vertex[2];
                    vertices[offset++] = _color[0];
                    vertices[offset++] = _color[1];
                    vertices[offset++] = _color[2];
                    vertices[offset++] = _color[3];
                    vertices[offset++] = xtc[i];
                    vertices[offset++] = ytc[j];
                }
            }

            std::vector<VertexAttribute> attributes;
            attributes.reserve(3);
            attributes.emplace_back(0, 3);
            attributes.emplace_back(1, 4);
            attributes.emplace_back(2, 2);

            vb.Create(vertices, verticesSize, attributes);
            ib.Create(indices, indicesSize);
        }
        else
        {
            glm::vec2 texCoords[4];
            if (_image->GetIsSpriteSheet())
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
            if (FlipImage)
            {
                std::swap(texCoords[0], texCoords[1]);
                std::swap(texCoords[2], texCoords[3]);
            }

            glm::vec3 vertices[4];
            vertices[0] = matrix * glm::vec4(1.0f - _image->GetPivot().x, 1.0f - _image->GetPivot().y, 0.0f, 1.0f);
            vertices[1] = matrix * glm::vec4(1.0f - _image->GetPivot().x, 0.0f - _image->GetPivot().y, 0.0f, 1.0f);
            vertices[2] = matrix * glm::vec4(0.0f - _image->GetPivot().x, 1.0f - _image->GetPivot().y, 0.0f, 1.0f);
            vertices[3] = matrix * glm::vec4(0.0f - _image->GetPivot().x, 0.0f - _image->GetPivot().y, 0.0f, 1.0f);

            auto indices = new uint32_t[6]{ 0, 1, 2, 1, 2, 3 };

            vb.Create(vertices, _color, texCoords);
            ib.Create(indices, 6);
        }
    }
}
