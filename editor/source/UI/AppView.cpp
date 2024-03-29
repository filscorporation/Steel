#include "AppView.h"
#include "StatsWindow.h"
#include "UIEditorTab.h"
#include "../EditorCore/EditorApplication.h"
#include "../EditorCore/EditorBuilder.h"

#include <Steel.h>
#include <Steel/Serialization/AttributesRegistration.h>

#undef LoadImage

void AppView::RegisterType()
{
    REGISTER_COMPONENT(AppView);
}

void AppView::OnEnabled(EntitiesRegistry* entitiesRegistry)
{
    UpdateView(entitiesRegistry);
    UpdateResolutionInfo(entitiesRegistry);
}

void AppView::OnDisabled(EntitiesRegistry* entitiesRegistry)
{
    auto editor = (EditorApplication*)Application::Instance;
    editor->GetAppContext()->IgnoreEvents = true;
}

void AppView::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    if (appViewImageEntity == NULL_ENTITY || !entitiesRegistry->HasComponent<UIImage>(appViewImageEntity))
        return;
    auto& image = entitiesRegistry->GetComponent<UIImage>(appViewImageEntity);
    auto oldImage = image.GetImage();
    image.SetImage(nullptr);
    delete oldImage;
}

void AppView::Init(EntitiesRegistry* entitiesRegistry)
{
    auto editor = (EditorApplication*)Application::Instance;
    auto scene = Application::Instance->GetCurrentScene();
    auto layer = scene->GetUILayer();

    EntityID frameEntity = layer->CreateUIImage(layer->UIResources.StraightFrameSprite, "Frame", Owner);
    auto& frameRT = entitiesRegistry->GetComponent<RectTransformation>(frameEntity);
    frameRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    frameRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));

    {
        auto sprite = new Sprite(editor->ApplicationFramebuffer->GetColorAttachment());
        appViewImageEntity = layer->CreateUIImage(nullptr, "App view image", frameEntity);
        entitiesRegistry->GetComponent<UIEventHandler>(appViewImageEntity).Type = EventHandlerTypes::Transparent;
        auto& viewImage = entitiesRegistry->GetComponent<UIImage>(appViewImageEntity);
        viewImage.FlipImage = true;
        viewImage.SetImage(sprite);
        auto& viewImageRT = entitiesRegistry->GetComponent<RectTransformation>(appViewImageEntity);
        viewImageRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        viewImageRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        viewImageRT.SetOffsetMin(glm::vec2(STYLE_OFFSET, STYLE_OFFSET));
        viewImageRT.SetOffsetMax(glm::vec2(STYLE_OFFSET, STYLE_OFFSET + STYLE_BUTTON_H * 1.2f));
    }

    {
        statsEntity = layer->CreateUIElement("Stats", appViewImageEntity);
        auto& statsRT = entitiesRegistry->GetComponent<RectTransformation>(statsEntity);
        statsRT.SetAnchorMin(glm::vec2(1.0f, 1.0f));
        statsRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        statsRT.SetPivot(glm::vec2(1.0f, 1.0f));
        statsRT.SetSize(glm::vec2(200, 74));
        statsRT.SetAnchoredPosition(glm::vec2(-4, -4));
        auto& stats = entitiesRegistry->AddComponent<StatsWindow>(statsEntity);
        stats.Init();
        entitiesRegistry->EntitySetActive(statsEntity, false, true);
    }

    {
        tabEntity = layer->CreateUIImage(layer->UIResources.DefaultPixelSprite, "Tab image", frameEntity);
        auto& tabImage = entitiesRegistry->GetComponent<UIImage>(tabEntity);
        tabImage.SetColor(STYLE_DARK_GREY);
        auto& tabImageRT = entitiesRegistry->GetComponent<RectTransformation>(tabEntity);
        tabImageRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        tabImageRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        tabImageRT.SetOffsetMin(glm::vec2(STYLE_OFFSET, 0.0f));
        tabImageRT.SetOffsetMax(glm::vec2(STYLE_OFFSET, 0.0f));
        tabImageRT.SetAnchoredPosition(glm::vec2(0.0f, -STYLE_OFFSET - STYLE_BUTTON_H * 1.2f * 0.5f));
        tabImageRT.SetSize(glm::vec2(0.0f, STYLE_BUTTON_H * 1.2f));

        {
            float x = 4.0f;
            {
                EntityID entity = layer->CreateUIText("Resolution:", "Resolution", tabEntity);
                auto& labelRT = entitiesRegistry->GetComponent<RectTransformation>(entity);
                labelRT.SetSize(glm::vec2(STYLE_BUTTON_H * 3.5f, STYLE_BUTTON_H));
                labelRT.SetPivot(glm::vec2(0.0f, 0.5f));
                labelRT.SetAnchorMin(glm::vec2(0.0f, 0.5f));
                labelRT.SetAnchorMax(glm::vec2(0.0f, 0.5f));
                labelRT.SetAnchoredPosition(glm::vec2(x, 0.0f));
                x += labelRT.GetSize().x;
            }

            {
                xFieldEntity = layer->CreateUIInputField("ResolutionX", tabEntity);
                auto& fieldRT = entitiesRegistry->GetComponent<RectTransformation>(xFieldEntity);
                fieldRT.SetSize(glm::vec2(STYLE_BUTTON_H * 3.5f, STYLE_BUTTON_H));
                fieldRT.SetPivot(glm::vec2(0.0f, 0.5f));
                fieldRT.SetAnchorMin(glm::vec2(0.0f, 0.5f));
                fieldRT.SetAnchorMax(glm::vec2(0.0f, 0.5f));
                fieldRT.SetAnchoredPosition(glm::vec2(x, 0.0f));
                auto& field = entitiesRegistry->GetComponent<UIInputField>(xFieldEntity);
                field.SetTextType(TextTypes::IntegerNumber);
                field.SetInteractable(false);
                xTextEntity = field.GetTargetText();
                auto& textRT = entitiesRegistry->GetComponent<RectTransformation>(xTextEntity);
                textRT.SetOffsetMin(glm::vec2(8, 2));
                textRT.SetOffsetMax(glm::vec2(8, 2));
                auto& fieldText = entitiesRegistry->GetComponent<UIText>(xTextEntity);
                fieldText.SetText(std::to_string(editor->GetAppContext()->ScreenParams.ResolutionX));
                field.SubmitCallback = [](EntityID entityID, const std::string& text)
                {
                    int value;
                    if (UIInputField::IsInt(text, value))
                    {
                        auto editor = (EditorApplication*)Application::Instance;
                        editor->GetAppContext()->ScreenParams.ResolutionX = glm::clamp(value, 0, 10000);
                        editor->GetAppContext()->ScreenParams.IsDirty = true;
                    }
                };

                x += fieldRT.GetSize().x;
            }

            {
                yFieldEntity = layer->CreateUIInputField("ResolutionY", tabEntity);
                auto& fieldRT = entitiesRegistry->GetComponent<RectTransformation>(yFieldEntity);
                fieldRT.SetSize(glm::vec2(STYLE_BUTTON_H * 3.5f, STYLE_BUTTON_H));
                fieldRT.SetPivot(glm::vec2(0.0f, 0.5f));
                fieldRT.SetAnchorMin(glm::vec2(0.0f, 0.5f));
                fieldRT.SetAnchorMax(glm::vec2(0.0f, 0.5f));
                fieldRT.SetAnchoredPosition(glm::vec2(x, 0.0f));
                auto& field = entitiesRegistry->GetComponent<UIInputField>(yFieldEntity);
                field.SetTextType(TextTypes::IntegerNumber);
                field.SetInteractable(false);
                yTextEntity = field.GetTargetText();
                auto& textRT = entitiesRegistry->GetComponent<RectTransformation>(yTextEntity);
                textRT.SetOffsetMin(glm::vec2(8, 2));
                textRT.SetOffsetMax(glm::vec2(8, 2));
                auto& fieldText = entitiesRegistry->GetComponent<UIText>(yTextEntity);
                fieldText.SetText(std::to_string(editor->GetAppContext()->ScreenParams.ResolutionY));
                field.SubmitCallback = [](EntityID entityID, const std::string& text)
                {
                    int value;
                    if (UIInputField::IsInt(text, value))
                    {
                        auto editor = (EditorApplication*)Application::Instance;
                        editor->GetAppContext()->ScreenParams.ResolutionY = glm::clamp(value, 0, 10000);
                        editor->GetAppContext()->ScreenParams.IsDirty = true;
                    }
                };
                x += fieldRT.GetSize().x;
            }

            {
                autoCBEntity = layer->CreateUICheckBox("auto", "Auto checkbox", tabEntity);
                auto& checkboxRT = entitiesRegistry->GetComponent<RectTransformation>(autoCBEntity);
                checkboxRT.SetPivot(glm::vec2(0.0f, 0.5f));
                checkboxRT.SetAnchorMin(glm::vec2(0.0f, 0.5f));
                checkboxRT.SetAnchorMax(glm::vec2(0.0f, 0.5f));
                checkboxRT.SetAnchoredPosition(glm::vec2(x, 0.0f));
                auto& autoCheckbox = entitiesRegistry->GetComponent<UICheckBox>(autoCBEntity);
                autoCheckbox.SetValue(editor->GetAppContext()->ScreenParams.AutoResolution);
                EntityID labelEntity = entitiesRegistry->GetComponent<HierarchyNode>(
                        entitiesRegistry->GetComponent<HierarchyNode>(autoCBEntity).GetFirstChildNode()).GetNextNode();
                entitiesRegistry->GetComponent<UIText>(labelEntity).SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

                autoCheckbox.Callback = [](EntityID entityID)
                {
                    auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
                    bool value = registry->GetComponent<UICheckBox>(entityID).GetValue();
                    auto editor = (EditorApplication*)Application::Instance;
                    editor->GetAppContext()->ScreenParams.AutoResolution = value;
                    editor->GetAppContext()->ScreenParams.IsDirty = true;
                };
                x += checkboxRT.GetSize().x;
            }
        }

        {
            EntityID entity = layer->CreateUICheckBox("Stats", "Stats checkbox", tabEntity);
            auto& checkboxRT = entitiesRegistry->GetComponent<RectTransformation>(entity);
            checkboxRT.SetPivot(glm::vec2(1.0f, 0.5f));
            checkboxRT.SetAnchorMin(glm::vec2(1.0f, 0.5f));
            checkboxRT.SetAnchorMax(glm::vec2(1.0f, 0.5f));
            auto& statsCheckbox = entitiesRegistry->GetComponent<UICheckBox>(entity);
            statsCheckbox.SetValue(false);
            EntityID labelEntity = entitiesRegistry->GetComponent<HierarchyNode>(
                    entitiesRegistry->GetComponent<HierarchyNode>(entity).GetFirstChildNode()).GetNextNode();
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
    auto editor = (EditorApplication*)Application::Instance;
    auto& viewRT = entitiesRegistry->GetComponent<RectTransformation>(Owner);

    EntityID parentEntity = entitiesRegistry->GetComponent<HierarchyNode>(Owner).GetParentNode();
    editor->GetAppContext()->IgnoreEvents = !entitiesRegistry->GetComponent<UIEditorTab>(parentEntity).GetIsFocused();

    if (viewRT.DidSizeChange() || editor->GetAppContext()->ScreenParams.IsDirty)
    {
        UpdateView(entitiesRegistry);
    }

    if (Screen::IsScreenSizeDirty() || editor->GetAppContext()->ScreenParams.IsDirty)
    {
        UpdateResolutionInfo(entitiesRegistry);
    }
}

void AppView::UpdateView(EntitiesRegistry* entitiesRegistry) const
{
    auto editor = (EditorApplication*)Application::Instance;
    auto& image = entitiesRegistry->GetComponent<UIImage>(appViewImageEntity);
    auto& rt = entitiesRegistry->GetComponent<RectTransformation>(appViewImageEntity);
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

void AppView::UpdateResolutionInfo(EntitiesRegistry* entitiesRegistry) const
{
    auto editor = (EditorApplication*)Application::Instance;
    auto layer = editor->GetCurrentScene()->GetUILayer();
    entitiesRegistry->GetComponent<UIText>(xTextEntity).SetText(std::to_string(editor->GetAppContext()->ScreenParams.ResolutionX));
    entitiesRegistry->GetComponent<UIText>(xTextEntity).Rebuild(layer, entitiesRegistry->GetComponent<RectTransformation>(xTextEntity), false, false);
    entitiesRegistry->GetComponent<UIText>(yTextEntity).SetText(std::to_string(editor->GetAppContext()->ScreenParams.ResolutionY));
    entitiesRegistry->GetComponent<UIText>(yTextEntity).Rebuild(layer, entitiesRegistry->GetComponent<RectTransformation>(yTextEntity), false, false);

    entitiesRegistry->GetComponent<UICheckBox>(autoCBEntity).SetValue(editor->GetAppContext()->ScreenParams.AutoResolution);
    entitiesRegistry->GetComponent<UIInputField>(xFieldEntity).SetInteractable(!editor->GetAppContext()->ScreenParams.AutoResolution);
    entitiesRegistry->GetComponent<UIInputField>(yFieldEntity).SetInteractable(!editor->GetAppContext()->ScreenParams.AutoResolution);
}
