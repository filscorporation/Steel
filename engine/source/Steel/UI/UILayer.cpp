#include "UILayer.h"
#include "UIElements/UIButton.h"
#include "UIElements/UIClipping.h"
#include "UIElements/UICheckBox.h"
#include "UIElements/UIInputField.h"
#include "UIElements/UILayoutGroup.h"
#include "UIElements/UITabs.h"
#include "../Core/Application.h"
#include "../Core/Log.h"
#include "../Rendering/Renderer.h"
#include "../Scene/Hierarchy.h"
#include "../Scene/NameComponent.h"

UILayer::UILayer(Scene* scene)
{
    _scene = scene;
}

void UILayer::LoadDefaultResources()
{
    auto resourcesManager = Application::Instance->GetResourcesManager();
    PixelsPerUnit = resourcesManager->GetDefaultPixelsPerUnit();

    UIResources.DefaultFrameSprite = resourcesManager->LoadSprite("frame.png", true);
    UIResources.DefaultFrameSprite->SetAsSliced(6);
    UIResources.StraightFrameSprite = resourcesManager->LoadSprite("straight_frame.png", true);
    UIResources.StraightFrameSprite->SetAsSliced(6);
    UIResources.DefaultPixelSprite = resourcesManager->LoadSprite("pixel.png", true);
    UIResources.DefaultStylePixelSprite = resourcesManager->LoadSprite("style_pixel.png", true);
    UIResources.DefaultArrowSprite = resourcesManager->LoadSprite("arrow.png", true);
    UIResources.DefaultCheckBoxSprite = resourcesManager->LoadSprite("check_box.png", true);
    UIResources.DefaultCheckBoxSprite->SetAsSliced(2);
    UIResources.DefaultCheckMarkSprite = resourcesManager->LoadSprite("check_mark.png", true);
    UIResources.DefaultTabOpenedSprite = resourcesManager->LoadSprite("tab_opened.png", true);
    UIResources.DefaultTabClosedSprite = resourcesManager->LoadSprite("tab_closed.png", true);
    UIResources.DefaultTabFocusedSprite = resourcesManager->LoadSprite("tab_focused.png", true); // TODO: move to editor
}

void UILayer::Update()
{
    auto entitiesRegistry = _scene->GetEntitiesRegistry();

    if (_updateQueue.Size() != 0)
    {
        for (int i = _updateQueue.Size() - 1; i >= 0; --i)
        {
            if (!_updateQueue[i].Callback(_updateQueue[i].Owner))
                _updateQueue.Remove(Entity::EntityIDGetID(_updateQueue[i].Owner));
        }
        _updateQueue.Condense();
    }

    auto uiInputFields = entitiesRegistry->GetComponentIterator<UIInputField>();
    for (int i = 0; i < uiInputFields.Size(); ++i)
        uiInputFields[i].Update();
}

void UILayer::Rebuild()
{
    auto entitiesRegistry = _scene->GetEntitiesRegistry();
    // TODO: support non default thickness
    _layerThickness = entitiesRegistry->GetComponentsCount<RectTransformation>() + 1;

    // Update rect transformations if needed
    auto hierarchyNodes = entitiesRegistry->GetComponentIterator<HierarchyNode>();
    auto rtAccessor = entitiesRegistry->GetComponentAccessor<RectTransformation>();
    // Components to apply changed transformation
    auto ehAccessor = entitiesRegistry->GetComponentAccessor<UIEventHandler>();
    auto imageAccessor = entitiesRegistry->GetComponentAccessor<UIImage>();
    auto textAccessor = entitiesRegistry->GetComponentAccessor<UIText>();
    auto hlAccessor = entitiesRegistry->GetComponentAccessor<UILayoutGroup>();
    for (auto& hierarchyNode : hierarchyNodes)
    {
        if (rtAccessor.Has(hierarchyNode.Owner))
        {
            auto& rt = rtAccessor.Get(hierarchyNode.Owner);
            rt.UpdateTransformation(this, rtAccessor, hierarchyNode);

            bool transformationDirty = rt.DidTransformationChange();
            if ((transformationDirty || _rebuildSortingOrder) && ehAccessor.Has(hierarchyNode.Owner))
                ehAccessor.Get(hierarchyNode.Owner).Update(entitiesRegistry, rt);
            if ((transformationDirty || _rebuildSortingOrder) && imageAccessor.Has(hierarchyNode.Owner))
                imageAccessor.Get(hierarchyNode.Owner).Rebuild(rt, transformationDirty, _rebuildSortingOrder);
            if (textAccessor.Has(hierarchyNode.Owner))
                // Possible place for optimization - entering component even if transformation not dirty
                textAccessor.Get(hierarchyNode.Owner).Rebuild(this, rt, transformationDirty, _rebuildSortingOrder);
            if (transformationDirty && hlAccessor.Has(hierarchyNode.Owner))
                hlAccessor.Get(hierarchyNode.Owner).Rebuild(this, rt);
        }
    }

    // Rebuild elements after rt update
    // TODO: need refactoring to regulate UI components update order

    // Clipping requires element thickness to be calculated (for closing cap)
    auto uiClippings = entitiesRegistry->GetComponentIterator<UIClipping>();
    for (int i = 0; i < uiClippings.Size(); ++i)
        uiClippings[i].Rebuild(this, rtAccessor.Get(uiClippings[i].Owner), _rebuildSortingOrder);

    // Input fields requires target text to be updated
    auto uiIFs = entitiesRegistry->GetComponentIterator<UIInputField>();
    for (int i = 0; i < uiIFs.Size(); ++i)
        uiIFs[i].Rebuild(rtAccessor.Get(uiIFs[i].Owner));
}

void UILayer::Refresh()
{
    auto entitiesRegistry = _scene->GetEntitiesRegistry();

    // Refresh rect transformation
    auto rectTransformations = entitiesRegistry->GetComponentIterator<RectTransformation>();
    for (int i = 0; i < rectTransformations.Size(); ++i)
        rectTransformations[i].RefreshTransformation();
    auto uiTexts = entitiesRegistry->GetComponentIterator<UIText>();
    for (int i = 0; i < uiTexts.Size(); ++i)
        uiTexts[i].Refresh();
    _currentHierarchyIndex = 0;
    _rebuildSortingOrder = false;
}

void UILayer::PollEvent(UIEvent& uiEvent)
{
    auto entitiesRegistry = _scene->GetEntitiesRegistry();
    auto rtAccessor = entitiesRegistry->GetComponentAccessor<RectTransformation>();
    auto uiEventHandlers = entitiesRegistry->GetComponentIterator<UIEventHandler>();
    // Sort event handlers by sorting order
    struct
    {
        bool operator()(UIEventHandler& a, UIEventHandler& b) const
        { return a.SortingOrder < b.SortingOrder; }
    } SOComparer;
    entitiesRegistry->SortComponents<UIEventHandler>(SOComparer);

    int size = uiEventHandlers.Size();
    for (int i = 0; i < size; ++i)
        if (uiEventHandlers[i].IsAlive())
            uiEventHandlers[i].HandleEvent(rtAccessor, uiEvent);

    _isPointerOverUI = uiEvent.Used;
}

void UILayer::AddToUpdateQueue(EntityID entityID, UpdateInteractable callback)
{
    auto id = Entity::EntityIDGetID(entityID);
    if (!_updateQueue.Has(id))
        _updateQueue.Add(id, callback, entityID);
}

void UILayer::RemoveFromUpdateQueue(EntityID entityID)
{
    auto id = Entity::EntityIDGetID(entityID);
    if (_updateQueue.Has(id))
        _updateQueue.Remove(id);
    _updateQueue.Condense();
}

bool UILayer::IsPointerOverUI() const
{
    return _isPointerOverUI;
}

UILayer* UILayer::Current()
{
    return Application::Instance->GetCurrentScene()->GetUILayer();
}

uint32_t UILayer::GetLayerThickness() const
{
    return _layerThickness;
}

uint32_t UILayer::GetCurrentHierarchyIndex() const
{
    return _currentHierarchyIndex;
}

void UILayer::IncreaseCurrentHierarchyIndex(uint32_t thickness)
{
    _currentHierarchyIndex += thickness;
}

void UILayer::SetSortingOrderDirty()
{
    _rebuildSortingOrder = true;
}

bool UILayer::NeedRebuildSortingOrder() const
{
    return _rebuildSortingOrder;
}

EntityID UILayer::CreateUIElement()
{
    return CreateUIElement("New UI element", NULL_ENTITY);
}

EntityID UILayer::CreateUIElement(const std::string& name, EntityID parent)
{
    auto entity = _scene->CreateEmptyEntity();
    auto& nameComponent = _scene->GetEntitiesRegistry()->AddComponent<NameComponent>(entity);
    nameComponent.SetName(name);
    _scene->GetEntitiesRegistry()->AddComponent<RectTransformation>(entity);
    _scene->GetEntitiesRegistry()->AddComponent<HierarchyNode>(entity);
    LinkChildToParent(_scene->GetEntitiesRegistry(), entity, parent);

    return entity;
}

EntityID UILayer::CreateUIImage()
{
    auto entity = CreateUIElement();
    _scene->GetEntitiesRegistry()->AddComponent<UIImage>(entity);

    return entity;
}

EntityID UILayer::CreateUIImage(Sprite* sprite, const std::string& name, EntityID parent)
{
    auto entity = CreateUIElement(name, parent);
    _scene->GetEntitiesRegistry()->AddComponent<UIImage>(entity).SetImage(sprite);

    return entity;
}

EntityID UILayer::CreateUIButton()
{
    auto entity = CreateUIImage();
    auto& button = _scene->GetEntitiesRegistry()->AddComponent<UIButton>(entity);
    button.SetTargetImage(entity);

    return entity;
}

EntityID UILayer::CreateUIButton(Sprite* sprite, const std::string& name, EntityID parent)
{
    auto entity = CreateUIImage(sprite, name, parent);
    auto& button = _scene->GetEntitiesRegistry()->AddComponent<UIButton>(entity);
    button.SetTargetImage(entity);

    return entity;
}

EntityID UILayer::CreateUIText()
{
    auto entity = CreateUIElement();
    _scene->GetEntitiesRegistry()->AddComponent<UIText>(entity);

    return entity;
}

EntityID UILayer::CreateUIText(const std::string& text, const std::string& name, EntityID parent)
{
    auto entity = CreateUIElement(name, parent);
    _scene->GetEntitiesRegistry()->AddComponent<UIText>(entity).SetText(text);

    return entity;
}

EntityID UILayer::CreateUIInputField()
{
    return CreateUIInputField("Input field", NULL_ENTITY);
}

EntityID UILayer::CreateUIInputField(const std::string& name, EntityID parent)
{
    auto entity = CreateUIElement(name, parent);
    auto& uiImage = _scene->GetEntitiesRegistry()->AddComponent<UIImage>(entity);
    uiImage.SetImage(UIResources.DefaultFrameSprite);
    auto& inputField = _scene->GetEntitiesRegistry()->AddComponent<UIInputField>(entity);
    inputField.SetTargetImage(uiImage.Owner);

    auto textEntity = CreateUIText("", "Text", entity);
    auto& uiText = _scene->GetEntitiesRegistry()->GetComponent<UIText>(textEntity);
    uiText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    auto& textRT = _scene->GetEntitiesRegistry()->GetComponent<RectTransformation>(textEntity);
    textRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    textRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    float offset = 6.0f;
    textRT.SetOffsetMin(glm::vec2(offset, offset));
    textRT.SetOffsetMax(glm::vec2(offset, offset));
    inputField.SetTargetText(textEntity);

    return entity;
}

EntityID UILayer::CreateUIClipping()
{
    auto entity = CreateUIElement();
    _scene->GetEntitiesRegistry()->AddComponent<UIClipping>(entity);

    return entity;
}

EntityID UILayer::CreateUIClipping(EntityID parent)
{
    auto entity = CreateUIElement("Clipping", parent);
    _scene->GetEntitiesRegistry()->AddComponent<UIClipping>(entity);

    return entity;
}

EntityID UILayer::CreateUICheckBox()
{
    return CreateUICheckBox("", "Check box", NULL_ENTITY);
}

EntityID UILayer::CreateUICheckBox(const std::string& label, const std::string& name, EntityID parent)
{
    float boxSize = 18;
    float markSize = 14;

    auto entity = CreateUIElement(name, parent);
    auto& checkBox = _scene->GetEntitiesRegistry()->AddComponent<UICheckBox>(entity);
    auto& entityRT = _scene->GetEntitiesRegistry()->GetComponent<RectTransformation>(entity);
    entityRT.SetSize(glm::vec2(boxSize * 5, boxSize));

    auto boxEntity = CreateUIImage(UIResources.DefaultCheckBoxSprite, "Check box frame", entity);
    auto& boxRT = _scene->GetEntitiesRegistry()->GetComponent<RectTransformation>(boxEntity);
    boxRT.SetSize(glm::vec2(boxSize, boxSize));
    boxRT.SetAnchorMin(glm::vec2(0.0f, 0.5f));
    boxRT.SetAnchorMax(glm::vec2(0.0f, 0.5f));
    boxRT.SetAnchoredPosition(glm::vec2(boxSize * 0.6f, 0));
    _scene->GetEntitiesRegistry()->GetComponent<UIEventHandler>(boxEntity).Type = EventHandlerTypes::Transparent;

    auto markEntity = CreateUIImage(UIResources.DefaultCheckMarkSprite, "Check mark", boxEntity);
    auto& markRT = _scene->GetEntitiesRegistry()->GetComponent<RectTransformation>(markEntity);
    markRT.SetAnchorMin(glm::vec2(0.5f, 0.5f));
    markRT.SetAnchorMax(glm::vec2(0.5f, 0.5f));
    markRT.SetSize(glm::vec2(markSize, markSize));
    _scene->GetEntitiesRegistry()->GetComponent<UIEventHandler>(markEntity).Type = EventHandlerTypes::Transparent;
    _scene->GetEntitiesRegistry()->EntitySetActive(markEntity, false, true);

    auto textEntity = CreateUIText(label, "Text", entity);
    auto& uiText = _scene->GetEntitiesRegistry()->GetComponent<UIText>(textEntity);
    uiText.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    uiText.SetTextSize(16);
    auto& textRT = _scene->GetEntitiesRegistry()->GetComponent<RectTransformation>(textEntity);
    textRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    textRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    textRT.SetOffsetMin(glm::vec2(boxSize * 1.2f, 0));
    textRT.SetOffsetMax(glm::vec2(0, 0));

    checkBox.checkMark = markEntity;
    checkBox.SetTargetImage(boxEntity);

    return entity;
}

EntityID UILayer::CreateUITabs()
{
    std::vector<std::string> tabs;
    return CreateUITabs(tabs, "Tabs", NULL_ENTITY);
}

EntityID UILayer::CreateUITabs(const std::vector<std::string>& tabsNames, const std::string& name, EntityID parent)
{
    auto entity = CreateUIElement(name, parent);
    _scene->GetEntitiesRegistry()->AddComponent<UIClipping>(entity);
    auto& tabs = _scene->GetEntitiesRegistry()->AddComponent<UITabs>(entity);

    for (auto& tabName : tabsNames)
        tabs.AddTab(tabName);

    return entity;
}

EntityID UILayer::CreateUILayoutGroup(LayoutGroupTypes::LayoutGroupType type)
{
    return CreateUILayoutGroup(type, "Layout group", NULL_ENTITY);
}

EntityID UILayer::CreateUILayoutGroup(LayoutGroupTypes::LayoutGroupType type, const std::string& name, EntityID parent)
{
    auto entity = CreateUIElement(name, parent);
    auto& layout = _scene->GetEntitiesRegistry()->AddComponent<UILayoutGroup>(entity);
    layout.SetType(type);

    return entity;
}
