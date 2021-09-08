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
    if (Input::IsKeyJustPressed(KeyCodes::Space))
    {
        // TODO: temp
        auto editor = (EditorApplication*)Application::Instance;
        editor->State = editor->State == EditorStates::Paused ? EditorStates::Playing : EditorStates::Paused;
    }

    auto& viewRT = entitiesRegistry->GetComponent<RectTransformation>(Owner);
    if (viewRT.DidSizeChange())
    {
        auto editor = (EditorApplication*)Application::Instance;
        auto& image = entitiesRegistry->GetComponent<UIImage>(appViewImageEntity);
        auto& rt = entitiesRegistry->GetComponent<RectTransformation>(Owner);

        auto& screenParameters = editor->GetAppContext()->ScreenParameters;
        screenParameters.ResolutionX = (int)rt.GetRealSizeCached().x; // TODO: can be independent app size
        screenParameters.ResolutionY = (int)rt.GetRealSizeCached().y;
        screenParameters.Width = (int)rt.GetRealSizeCached().x;
        screenParameters.Height = (int)rt.GetRealSizeCached().y;
        screenParameters.OffsetX = (int)(rt.GetRealPositionCached().x - (float)screenParameters.Width * 0.5f);
        screenParameters.OffsetY = (int)(rt.GetRealPositionCached().y - (float)screenParameters.Height * 0.5f);

        image.GetImage()->SpriteTexture = nullptr;
        delete image.GetImage();

        editor->ApplicationFramebuffer->Resize((uint32_t)rt.GetRealSizeCached().x, (uint32_t)rt.GetRealSizeCached().y);

        auto sprite = new Sprite(editor->ApplicationFramebuffer->GetColorAttachment());
        image.FlipImage = true;
        image.SetImage(sprite);
    }
}
