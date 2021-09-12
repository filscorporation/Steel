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
        glm::vec2 rectSize = rt.GetRealSizeCached();

        auto& screenParameters = editor->GetAppContext()->ScreenParameters;
        screenParameters.ResolutionX = (int)rectSize.x; // TODO: can be independent app size
        screenParameters.ResolutionY = (int)rectSize.y;
        screenParameters.Width = (int)rectSize.x;
        screenParameters.Height = (int)rectSize.y;
        screenParameters.OffsetX = (int)(rt.GetRealPositionCached().x - (float)screenParameters.Width * 0.5f);
        screenParameters.OffsetY = (int)(rt.GetRealPositionCached().y - (float)screenParameters.Height * 0.5f);

        delete image.GetImage();

        // TODO: fix bug when sprite renderers do not get draw on resize
        editor->ApplicationFramebuffer->Resize((uint32_t)screenParameters.ResolutionX, (uint32_t)screenParameters.ResolutionY);

        auto sprite = new Sprite(editor->ApplicationFramebuffer->GetColorAttachment());
        image.FlipImage = true;
        image.SetImage(sprite);
    }
}
