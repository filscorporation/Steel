#include "SceneView.h"
#include "UIEditorTab.h"
#include "../EditorCore/EditorApplication.h"
#include "../EditorCore/EditorBuilder.h"

void SceneView::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    UpdateView(entitiesRegistry);
}

void SceneView::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (sceneViewImageEntity == NULL_ENTITY || !entitiesRegistry->HasComponent<UIImage>(sceneViewImageEntity))
        return;
    auto& image = entitiesRegistry->GetComponent<UIImage>(sceneViewImageEntity);
    image.SetImage(nullptr);
    delete image.GetImage();
}

void SceneView::Init(EntitiesRegistry* entitiesRegistry)
{
    auto editor = (EditorApplication*)Application::Instance;
    auto scene = Application::Instance->GetCurrentScene();
    auto layer = scene->GetUILayer();

    EntityID frameEntity = layer->CreateUIImage(layer->UIResources.StraightFrameSprite, "Frame", Owner);
    auto& frameRT = entitiesRegistry->GetComponent<RectTransformation>(frameEntity);
    frameRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    frameRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));

    {
        // TODO: change to scene framebuffer
        auto sprite = new Sprite(editor->ApplicationFramebuffer->GetColorAttachment());
        sceneViewImageEntity = layer->CreateUIImage(nullptr, "Scene view image", frameEntity);
        entitiesRegistry->GetComponent<UIEventHandler>(sceneViewImageEntity).Type = EventHandlerTypes::Transparent;
        auto& viewImage = entitiesRegistry->GetComponent<UIImage>(sceneViewImageEntity);
        viewImage.FlipImage = true;
        viewImage.SetImage(sprite);
        auto& viewImageRT = entitiesRegistry->GetComponent<RectTransformation>(sceneViewImageEntity);
        viewImageRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        viewImageRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        viewImageRT.SetOffsetMin(glm::vec2(STYLE_OFFSET, STYLE_OFFSET));
        viewImageRT.SetOffsetMax(glm::vec2(STYLE_OFFSET, STYLE_OFFSET));
    }
}

void SceneView::Update(EntitiesRegistry* entitiesRegistry)
{
    auto editor = (EditorApplication*)Application::Instance;
    auto& viewRT = entitiesRegistry->GetComponent<RectTransformation>(Owner);

    EntityID parentEntity = entitiesRegistry->GetComponent<HierarchyNode>(Owner).ParentNode;
    ignoreInput = !entitiesRegistry->GetComponent<UIEditorTab>(parentEntity).GetIsFocused();

    if (viewRT.DidSizeChange() || editor->GetAppContext()->ScreenParams.IsDirty)
    {
        UpdateView(entitiesRegistry);
    }
}

void SceneView::UpdateView(EntitiesRegistry* entitiesRegistry) const
{
    return;
    auto editor = (EditorApplication*)Application::Instance;
    auto& image = entitiesRegistry->GetComponent<UIImage>(sceneViewImageEntity);
    auto& rt = entitiesRegistry->GetComponent<RectTransformation>(sceneViewImageEntity);
    glm::vec2 rectSize = rt.GetRealSizeCached();

    auto& screenParameters = editor->GetAppContext()->ScreenParams;
    if (editor->GetAppContext()->ScreenParams.AutoResolution)
    {
        editor->GetAppContext()->ScreenParams.ResolutionX = (int)rectSize.x;
        editor->GetAppContext()->ScreenParams.ResolutionY = (int)rectSize.y;
        editor->GetAppContext()->ScreenParams.IsDirty = true;
    }
    screenParameters.Width = (int)rectSize.x;
    screenParameters.Height = (int)rectSize.y;
    screenParameters.WidthBackup = (int)rectSize.x;
    screenParameters.HeightBackup = (int)rectSize.y;
    screenParameters.OffsetX = (int)(rt.GetRealPositionCached().x - (float)screenParameters.Width * 0.5f);
    screenParameters.OffsetY = (int)(rt.GetRealPositionCached().y - (float)screenParameters.Height * 0.5f);

    editor->ApplicationFramebuffer->Resize((uint32_t)screenParameters.ResolutionX, (uint32_t)screenParameters.ResolutionY);

    auto sprite = new Sprite(editor->ApplicationFramebuffer->GetColorAttachment());
    image.FlipImage = true;

    auto oldImage = image.GetImage();
    image.SetImage(sprite);
    delete oldImage;
}
