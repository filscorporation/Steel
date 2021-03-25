#include "UIRenderer.h"
#include "../Core/Application.h"
#include "../Rendering/Renderer.h"

void UIRenderer::Render(RectTransformation& rectTransformation)
{
    if (!IsRendered)
        return;

    if (rectTransformation.DidTransformationChange())
        Renderer::DrawQuad(quadCache, rectTransformation.GetTransformationMatrixCached(), Color, TextureID, TextureCoords);
    else
        Renderer::DrawQuadCached(quadCache, Color, TextureID, TextureCoords);

    rectTransformation.SetTransformationChanged(false);
}
