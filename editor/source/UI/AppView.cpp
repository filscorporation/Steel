#include "AppView.h"
#include "StatsWindow.h"
#include "../EditorCore/EditorApplication.h"
#include "../EditorCore/EditorBuilder.h"

#include <Iron.h>

#undef LoadImage

void AppView::Init(EntitiesRegistry* entitiesRegistry)
{
    auto editor = (EditorApplication*)Application::Instance;
    auto scene = Application::Instance->GetCurrentScene();

    {
        auto sprite = new Sprite(editor->ApplicationFramebuffer->GetColorAttachment());
        appViewImageEntity = scene->GetUILayer()->CreateUIImage(nullptr, "App view image", Owner);
        entitiesRegistry->GetComponent<UIEventHandler>(appViewImageEntity).Type = EventHandlerTypes::Transparent;
        auto& viewImage = entitiesRegistry->GetComponent<UIImage>(appViewImageEntity);
        viewImage.FlipImage = true;
        viewImage.SetImage(sprite);
        auto& viewImageRT = entitiesRegistry->GetComponent<RectTransformation>(appViewImageEntity);
        viewImageRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        viewImageRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        viewImageRT.SetOffsetMax(glm::vec2(0.0f, STYLE_BUTTON_H * 1.2f));
    }

    {
        statsEntity = scene->GetUILayer()->CreateUIElement("Stats", appViewImageEntity);
        auto& statsRT = entitiesRegistry->GetComponent<RectTransformation>(statsEntity);
        statsRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        statsRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        statsRT.SetPivot(glm::vec2(1.0f, 1.0f));
        statsRT.SetSize(glm::vec2(200, 94));
        statsRT.SetAnchoredPosition(glm::vec2(-4, -4));
        auto& stats = entitiesRegistry->AddComponent<StatsWindow>(statsEntity);
        stats.Init();
        entitiesRegistry->EntitySetActive(statsEntity, false, true);
    }

    {
        tabEntity = scene->GetUILayer()->CreateUIImage(scene->GetUILayer()->UIResources.DefaultPixelSprite, "Tab image", Owner);
        auto& tabImage = entitiesRegistry->GetComponent<UIImage>(tabEntity);
        tabImage.SetColor(STYLE_DARK_GREY);
        auto& tabImageRT = entitiesRegistry->GetComponent<RectTransformation>(tabEntity);
        tabImageRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        tabImageRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        tabImageRT.SetAnchoredPosition(glm::vec2(0.0f, -STYLE_BUTTON_H * 1.2f * 0.5f));
        tabImageRT.SetSize(glm::vec2(0.0f, STYLE_BUTTON_H * 1.2f));

        {
            EntityID entity = scene->GetUILayer()->CreateUICheckBox("Stats", "Stats checkbox", tabEntity);
            auto& checkboxRT = entitiesRegistry->GetComponent<RectTransformation>(entity);
            checkboxRT.SetPivot(glm::vec2(1.0f, 0.5f));
            checkboxRT.SetAnchorMin(glm::vec2(1.0f, 0.5f));
            checkboxRT.SetAnchorMax(glm::vec2(1.0f, 0.5f));
            auto& statsCheckbox = entitiesRegistry->GetComponent<UICheckBox>(entity);
            statsCheckbox.SetValue(false);
            EntityID labelEntity = entitiesRegistry->GetComponent<HierarchyNode>(
                    entitiesRegistry->GetComponent<HierarchyNode>(entity).FirstChildNode).NextNode;
            entitiesRegistry->GetComponent<UIText>(labelEntity).SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

            EntityID _statsEntity = statsEntity;
            statsCheckbox.Callback = [_statsEntity](EntityID entityID)
            {
                auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
                auto& checkbox = registry->GetComponent<UICheckBox>(entityID);
                registry->EntitySetActive(_statsEntity, checkbox.GetValue(), true);
            };
        }
    }
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
        auto& rt = entitiesRegistry->GetComponent<RectTransformation>(appViewImageEntity);
        glm::vec2 rectSize = rt.GetRealSizeCached();

        auto& screenParameters = editor->GetAppContext()->ScreenParams;
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
