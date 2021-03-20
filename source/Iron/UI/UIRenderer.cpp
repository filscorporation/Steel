#include "UIRenderer.h"
#include "../Core/Application.h"
#include "../Rendering/Renderer.h"

void UIRenderer::OnRender(RectTransformation& rectTransformation)
{
    if (_image == nullptr)
        return;

    if (rectTransformation.DidTransformationChange())
        Renderer::DrawQuad(quadCache, rectTransformation.GetTransformationMatrixCached(), _image->TextureID);
    else
        Renderer::DrawQuadCached(quadCache, _image->TextureID);

    rectTransformation.SetTransformationChanged(false);
}

bool UIRenderer::IsTransparent() const
{
    return _isTransparent;
}
