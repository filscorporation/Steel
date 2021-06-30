#include "UIImage.h"
#include "../UIQuadRenderer.h"
#include "../UIEventHandler.h"
#include "../../Core/Log.h"
#include "../../Scene/SceneHelper.h"
#include "../../Rendering/Screen.h"

void UIImage::UpdateRenderer(RectTransformation& transformation, bool transformationDirty, bool sortingOrderDirty)
{
    if (_image == nullptr)
        return;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    glm::mat4 matrix = transformation.GetTransformationMatrixCached();
    float sortingOrder = transformation.GetSortingOrder();

    if (_image->IsSliced)
    {
        float k = (float)_image->PixelsPerUnit / (float)UILayer::Current()->PixelsPerUnit;
        float xvs[4] = { 0.0f, (float)_image->SliceLeftOffset * k / transformation.GetSize().x,
                         1.0f - (float)_image->SliceRightOffset * k / transformation.GetSize().x, 1.0f };
        float yvs[4] = { 0.0f, (float)_image->SliceBottomOffset * k / transformation.GetSize().y,
                         1.0f - (float)_image->SliceTopOffset * k / transformation.GetSize().y, 1.0f };

        for (int n = 0; n < _renderers.size(); ++n)
        {
            auto& qr = registry->GetComponent<UIQuadRenderer>(_renderers[n]);

            if (transformation.DidSizeChange())
            {
                int i = n % 3, j = n / 3;
                qr.DefaultVertices[0] = glm::vec4(xvs[i + 1] - 0.5f, yvs[j] - 0.5f, 0.0f, 1.0f);
                qr.DefaultVertices[1] = glm::vec4(xvs[i + 1] - 0.5f, yvs[j + 1] - 0.5f, 0.0f, 1.0f);
                qr.DefaultVertices[2] = glm::vec4(xvs[i] - 0.5f, yvs[j] - 0.5f, 0.0f, 1.0f);
                qr.DefaultVertices[3] = glm::vec4(xvs[i] - 0.5f, yvs[j + 1] - 0.5f, 0.0f, 1.0f);
            }

            for (int m = 0; m < 4; ++m)
                qr.Vertices[m] = matrix * qr.DefaultVertices[m];
            qr.SortingOrder = sortingOrder;
        }
    }
    else
    {
        auto& qr = registry->GetComponent<UIQuadRenderer>(Owner);
        for (int i = 0; i < 4; ++i)
            qr.Vertices[i] = matrix * qr.DefaultVertices[i];
        qr.SortingOrder = sortingOrder;
    }

    if (registry->HasComponent<UIEventHandler>(Owner))
        registry->GetComponent<UIEventHandler>(Owner).SortingOrder = sortingOrder;
}

void UIImage::SetMaterial(Material* material)
{
    _material = material;
    SetImage(_image);
}

Material* UIImage::GetMaterial()
{
    return _material;
}

void UIImage::SetCustomProperties(const MaterialPropertyBlock& properties)
{
    _customProperties = properties;

    if (_image == nullptr || _material == nullptr)
        return;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (_image->IsSliced)
    {
        for (uint32_t _renderer : _renderers)
        {
            entitiesRegistry->GetComponent<UIQuadRenderer>(_renderer).CustomProperties = _customProperties;
        }
    }
    else
    {
        entitiesRegistry->GetComponent<UIQuadRenderer>(Owner).CustomProperties = _customProperties;
    }
}

const MaterialPropertyBlock& UIImage::GetCustomProperties()
{
    return _customProperties;
}

void UIImage::SetImage(Sprite* image)
{
    if (image != nullptr && _material == nullptr)
        _material = Application::Instance->GetResourcesManager()->DefaultUIMaterial();

    bool wasNull = _image == nullptr;
    _image = image;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& transformation = registry->GetComponent<RectTransformation>(Owner);
    if (_image == nullptr)
    {
        if (!wasNull)
        {
            for (auto renderer : _renderers)
                registry->DeleteEntity(renderer);
            _renderers.clear();
            registry->RemoveComponent<UIQuadRenderer>(Owner);
        }
    }
    else
    {
        if (image->IsSliced)
        {
            if (_renderers.empty())
            {
                _renderers.resize(9);
                for (int i = 0; i < 9; ++i)
                    _renderers[i] = registry->CreateNewEntity();
            }

            float xtc[4] = { 0.0f, (float)_image->SliceLeftOffset / (float)_image->Width,
                             1.0f - (float)_image->SliceRightOffset / (float)_image->Width, 1.0f };
            float ytc[4] = { 0.0f, (float)_image->SliceBottomOffset / (float)_image->Height,
                             1.0f - (float)_image->SliceTopOffset / (float)_image->Height, 1.0f };

            float k = (float)_image->PixelsPerUnit / (float)UILayer::Current()->PixelsPerUnit;
            float xvs[4] = { 0.0f, (float)_image->SliceLeftOffset * k / (float)Screen::GetWidth(),
                             1.0f - (float)_image->SliceRightOffset * k / (float)Screen::GetWidth(), 1.0f };
            float yvs[4] = { 0.0f, (float)_image->SliceBottomOffset * k / (float)Screen::GetHeight(),
                             1.0f - (float)_image->SliceTopOffset * k / (float)Screen::GetHeight(), 1.0f };

            for (int j = 0; j < 3; ++j)
            {
                for (int i = 0; i < 3; ++i)
                {
                    auto& qr = registry->AddComponent<UIQuadRenderer>(_renderers[j * 3 + i]);
                    qr.CustomOwner = Owner;
                    qr.RenderMaterial = _material;
                    qr.CustomProperties.SetTexture(MAIN_TEX, _image->TextureID);
                    qr.Color = _color;
                    qr.Queue = _image->IsTransparent ? RenderingQueue::Transparent : RenderingQueue::Opaque;
                    qr.TextureCoords[0] = glm::vec2(xtc[i + 1], ytc[j]);
                    qr.TextureCoords[1] = glm::vec2(xtc[i + 1], ytc[j + 1]);
                    qr.TextureCoords[2] = glm::vec2(xtc[i], ytc[j]);
                    qr.TextureCoords[3] = glm::vec2(xtc[i], ytc[j + 1]);
                    qr.DefaultVertices[0] = glm::vec4(xvs[i + 1] - 0.5f, yvs[j] - 0.5f, 0.0f, 1.0f);
                    qr.DefaultVertices[1] = glm::vec4(xvs[i + 1] - 0.5f, yvs[j + 1] - 0.5f, 0.0f, 1.0f);
                    qr.DefaultVertices[2] = glm::vec4(xvs[i] - 0.5f, yvs[j] - 0.5f, 0.0f, 1.0f);
                    qr.DefaultVertices[3] = glm::vec4(xvs[i] - 0.5f, yvs[j + 1] - 0.5f, 0.0f, 1.0f);
                }
            }
        }
        else
        {
            for (auto renderer : _renderers)
                registry->DeleteEntity(renderer);
            _renderers.clear();

            auto& qr = registry->AddComponent<UIQuadRenderer>(Owner);
            if (_image->IsSpriteSheet)
            {
                _image->GetTexCoord(currentImageTileIndex, qr.TextureCoords);
            }
            else
            {
                qr.TextureCoords[0] = glm::vec2(1.0f, 0.0f);
                qr.TextureCoords[1] = glm::vec2(1.0f, 1.0f);
                qr.TextureCoords[2] = glm::vec2(0.0f, 0.0f);
                qr.TextureCoords[3] = glm::vec2(0.0f, 1.0f);
            }
            qr.SetDefaultQuad();
            qr.Color = _color;
            qr.RenderMaterial = _material;
            qr.CustomProperties.SetTexture(MAIN_TEX, _image->TextureID);
            qr.Queue = _image->IsTransparent ? RenderingQueue::Transparent : RenderingQueue::Opaque;
            qr.CustomOwner = Owner;
        }
    }

    if (_image == nullptr)
        transformation.SetSize(glm::vec2(0.0f, 0.0f));
    else if (wasNull)
    {
        glm::vec2 size;
        size.x = _image->IsSpriteSheet ? _image->TileWidth : _image->Width;
        size.y = _image->IsSpriteSheet ? _image->TileHeight : _image->Height;
        transformation.SetSize(size);
    }
}

Sprite* UIImage::GetImage()
{
    return _image;
}

void UIImage::SetColor(glm::vec4 color)
{
    _color = color;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (_image->IsSliced)
    {
        for (uint32_t _renderer : _renderers)
        {
            entitiesRegistry->GetComponent<UIQuadRenderer>(_renderer).Color = _color;
        }
    }
    else
    {
        entitiesRegistry->GetComponent<UIQuadRenderer>(Owner).Color = _color;
    }
}

glm::vec4 UIImage::GetColor()
{
    return _color;
}

void UIImage::SetImageTileIndex(uint32_t index)
{
    if (_image->IsSliced)
    {
        // TODO: make it work for sliced
        Log::LogWarning("Setting tile index for sliced image is not supported yet");
        return;
    }

    if (index == currentImageTileIndex)
        return;
    currentImageTileIndex = index;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

    if (_image->IsSliced)
    {
        // TODO
        //for (uint32_t _renderer : _renderers)
        //{
        //    auto& qr = entitiesRegistry->GetComponent<UIQuadRenderer>(_renderer);
        //    _image->GetTexCoord(currentImageTileIndex, qr.TextureCoords);
        //}
    }
    else
    {
        auto& qr = entitiesRegistry->GetComponent<UIQuadRenderer>(Owner);
        _image->GetTexCoord(currentImageTileIndex, qr.TextureCoords);
    }
}

uint32_t UIImage::GetImageTileIndex()
{
    return currentImageTileIndex;
}
