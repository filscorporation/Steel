#include "AppView.h"
#include "../EditorCore/EditorApplication.h"
#include <Iron.h>

#undef LoadImage

void AppView::Init(EntitiesRegistry* entitiesRegistry)
{
    auto editor = (EditorApplication*)Application::Instance;

    auto scene = Application::Instance->GetCurrentScene();
    auto sprite = new Sprite(editor->ApplicationFramebuffer->GetColorAttachment());
    appViewImageEntity = scene->GetUILayer()->CreateUIImage(nullptr, "App view image", Owner);
    auto& image = scene->GetEntitiesRegistry()->GetComponent<UIImage>(appViewImageEntity);
    image.FlipImage = true;
    image.SetImage(sprite);
    auto& imageRT = scene->GetEntitiesRegistry()->GetComponent<RectTransformation>(appViewImageEntity);
    imageRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    imageRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
}

void AppView::Update(EntitiesRegistry* entitiesRegistry)
{
    auto& viewRT = entitiesRegistry->GetComponent<RectTransformation>(Owner);
    if (viewRT.DidSizeChange())
    {
        auto editor = (EditorApplication*)Application::Instance;
        auto& image = entitiesRegistry->GetComponent<UIImage>(appViewImageEntity);
        auto& rt = entitiesRegistry->GetComponent<RectTransformation>(Owner);

        editor->ApplicationWidth = (int)rt.GetRealSizeCached().x; // TODO: can be independent app size
        editor->ApplicationHeight = (int)rt.GetRealSizeCached().y;
        editor->ApplicationWindowWidth = (int)rt.GetRealSizeCached().x;
        editor->ApplicationWindowHeight = (int)rt.GetRealSizeCached().y;
        editor->ApplicationX = (int)(rt.GetRealPositionCached().x - (float)editor->ApplicationWidth * 0.5f);
        editor->ApplicationY = (int)(rt.GetRealPositionCached().y - (float)editor->ApplicationHeight * 0.5f);

        image.GetImage()->SpriteTexture = nullptr;
        delete image.GetImage();

        editor->ApplicationFramebuffer->Resize((uint32_t)rt.GetRealSizeCached().x, (uint32_t)rt.GetRealSizeCached().y);

        auto sprite = new Sprite(editor->ApplicationFramebuffer->GetColorAttachment());
        image.FlipImage = true;
        image.SetImage(sprite);
    }
}
