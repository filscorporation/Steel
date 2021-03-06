#include "UIRenderer.h"
#include "../Rendering/Renderer.h"
#include "../Scene/Entity.h"

void UIRenderer::OnRender()
{
    if (ParentEntity->IsDestroyed())
        return;

    if (_image == nullptr)
        return;

    glm::vec3 size = glm::vec3(_image->Width, _image->Height, 1.0f);
    glm::mat4 transformation = glm::scale(ParentEntity->Transform->GetTransformationMatrix(), size);

    // TODO: passing draw UI data logic
    Renderer::DrawQuad(transformation, _image->TextureID);
}
