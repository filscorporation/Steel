#include "UIRenderer.h"
#include "../Core/Application.h"
#include "../Rendering/Renderer.h"
#include "../Scene/SceneHelper.h"

void UIRenderer::OnRender()
{
    if (Application::Instance->GetCurrentScene()->IsEntityDestroyed(Owner))
        return;

    if (_image == nullptr)
        return;

    glm::mat4 transformation = GetComponentS<RectTransformation>(Owner).GetTransformationMatrix();

    // TODO: passing draw UI data logic
    Renderer::DrawQuad(transformation, _image->TextureID);
}
