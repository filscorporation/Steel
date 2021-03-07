#include "UIRenderer.h"
#include "../Rendering/Renderer.h"
#include "../Scene/Entity.h"

void UIRenderer::OnRender()
{
    if (ParentEntity->IsDestroyed())
        return;

    if (_image == nullptr)
        return;

    glm::mat4 transformation = ParentEntity->Transform->GetTransformationMatrix();

    // TODO: passing draw UI data logic
    Renderer::DrawQuad(transformation, _image->TextureID);
}
