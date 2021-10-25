#include "UIImage.h"
#include "../UIQuadRenderer.h"
#include "../UIEventHandler.h"
#include "../../Core/Log.h"
#include "../../Scene/Hierarchy.h"
#include "../../Scene/SceneHelper.h"
#include "../../Rendering/Screen.h"

bool UIImage::Validate(EntitiesRegistry* entitiesRegistry)
{
    return Component::Validate(entitiesRegistry) && CheckRectTransformation(entitiesRegistry);
}

void UIImage::OnCreated(EntitiesRegistry* entitiesRegistry)
{
    entitiesRegistry->AddComponent<UIEventHandler>(Owner);
    _clippingLevel = GetClippingLevelUpwards(entitiesRegistry, Owner);
}

void UIImage::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    for (auto qrID : _renderers)
        entitiesRegistry->DeleteEntity(qrID);
    _renderers.clear();
}

void UIImage::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    for (auto qrID : _renderers)
        entitiesRegistry->EntitySetActive(qrID, true, false);
}

void UIImage::OnDisabled(EntitiesRegistry* entitiesRegistry)
{
    for (auto qrID : _renderers)
        entitiesRegistry->EntitySetActive(qrID, false, false);
}

void UIImage::UpdateRenderer(RectTransformation& transformation, bool transformationDirty, bool sortingOrderDirty)
{
    if (_image == nullptr)
        return;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    glm::mat4 matrix = transformation.GetTransformationMatrixCached();
    float sortingOrder = transformation.GetSortingOrder();

    if (_image->IsSliced)
    {
        glm::vec2 size = transformation.GetRealSizeCached();
        float k = (float)_image->PixelsPerUnit / (float)UILayer::Current()->PixelsPerUnit;
        float xvs[4] = { 0.0f, (float)_image->SliceLeftOffset * k / size.x,
                         1.0f - (float)_image->SliceRightOffset * k / size.x, 1.0f };
        float yvs[4] = { 0.0f, (float)_image->SliceBottomOffset * k / size.y,
                         1.0f - (float)_image->SliceTopOffset * k / size.y, 1.0f };

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
    _customProperties.UpdateHash();

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
    _customProperties.UpdateHash();

    if (image != nullptr && _material == nullptr)
        _material = Application::Instance->GetResourcesManager()->DefaultUIMaterial();

    bool wasNull = _image == nullptr || _image->SpriteTexture == nullptr;
    _image = image;

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& transformation = registry->GetComponent<RectTransformation>(Owner);
    if (_image == nullptr || _image->SpriteTexture == nullptr)
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
        if (_image->IsSliced)
        {
            if (_renderers.empty())
            {
                _renderers.resize(9);
                for (int i = 0; i < 9; ++i)
                    _renderers[i] = registry->CreateNewEntity();
            }

            float xtc[4] = { 0.0f, (float)_image->SliceLeftOffset / (float)_image->SpriteTexture->GetWidth(),
                             1.0f - (float)_image->SliceRightOffset / (float)_image->SpriteTexture->GetWidth(), 1.0f };
            float ytc[4] = { 0.0f, (float)_image->SliceBottomOffset / (float)_image->SpriteTexture->GetHeight(),
                             1.0f - (float)_image->SliceTopOffset / (float)_image->SpriteTexture->GetHeight(), 1.0f };

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
                    _customProperties.SetTexture(MAIN_TEX, _image->SpriteTexture->GetTextureID());
                    _customProperties.SetStencilFunc(ComparisonFunctions::Equal, _clippingLevel, 255);
                    qr.CustomProperties = _customProperties;
                    qr.Color = _color;
                    qr.Queue = _image->IsTransparent ? RenderingQueue::Transparent : RenderingQueue::Opaque;
                    qr.TextureCoords[0] = glm::vec2(xtc[i + 1], ytc[j]);
                    qr.TextureCoords[1] = glm::vec2(xtc[i + 1], ytc[j + 1]);
                    qr.TextureCoords[2] = glm::vec2(xtc[i], ytc[j]);
                    qr.TextureCoords[3] = glm::vec2(xtc[i], ytc[j + 1]);
                    if (FlipImage)
                        qr.FlipTextureCoords();
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
            if (FlipImage)
                qr.FlipTextureCoords();
            qr.SetDefaultQuad();
            qr.Color = _color;
            qr.RenderMaterial = _material;
            _customProperties.SetTexture(MAIN_TEX, _image->SpriteTexture->GetTextureID());
            _customProperties.SetStencilFunc(ComparisonFunctions::Equal, _clippingLevel, 255);
            qr.CustomProperties = _customProperties;
            qr.Queue = _image->IsTransparent ? RenderingQueue::Transparent : RenderingQueue::Opaque;
            qr.CustomOwner = Owner;
        }
    }
}

Sprite* UIImage::GetImage()
{
    return _image;
}

void UIImage::SetColor(glm::vec4 color)
{
    _color = color;

    if (_image == nullptr)
        return;

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
    if (index == currentImageTileIndex)
        return;

    if (_image == nullptr || !_image->IsSpriteSheet)
        return;

    if (_image->IsSliced)
    {
        // TODO: make it work for sliced
        Log::LogWarning("Setting tile index for sliced image is not supported yet");
        return;
    }

    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& qr = registry->GetComponent<UIQuadRenderer>(Owner);

    currentImageTileIndex = index;
    _image->GetTexCoord(currentImageTileIndex, qr.TextureCoords);
}

uint32_t UIImage::GetImageTileIndex()
{
    return currentImageTileIndex;
}

void UIImage::SetClippingLevel(short clippingLevel)
{
    if (clippingLevel == _clippingLevel)
        return;

    if (_image == nullptr)
        return;

    _clippingLevel = clippingLevel;

    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    if (_image->IsSliced)
    {
        for (uint32_t _renderer : _renderers)
        {
            auto& qr = entitiesRegistry->GetComponent<UIQuadRenderer>(_renderer);
            qr.CustomProperties.SetStencilFunc(ComparisonFunctions::Equal, _clippingLevel, 255);
        }
    }
    else
    {
        auto& qr = entitiesRegistry->GetComponent<UIQuadRenderer>(Owner);
        qr.CustomProperties.SetStencilFunc(ComparisonFunctions::Equal, _clippingLevel, 255);
    }
}
