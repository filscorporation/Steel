#include "UILayer.h"
#include "UIElements/UIButton.h"
#include "UIElements/UIInputField.h"
#include "UISystem.h"
#include "../Core/Application.h"
#include "../Core/Log.h"
#include "../Rendering/Renderer.h"
#include "../Scene/Hierarchy.h"
#include "../Scene/NameComponent.h"

UILayer::UILayer(Scene* scene)
{
    _scene = scene;
    uiSystem = new UISystem();
    _scene->GetEntitiesRegistry()->RegisterSystem<UIText>(uiSystem);
    _scene->GetEntitiesRegistry()->RegisterSystem<UIImage>(uiSystem);
    _scene->GetEntitiesRegistry()->RegisterSystem<UIButton>(uiSystem);
    _scene->GetEntitiesRegistry()->RegisterSystem<UIInputField>(uiSystem);
}

UILayer::~UILayer()
{
    _scene->GetEntitiesRegistry()->RemoveSystem<UIText>();
    _scene->GetEntitiesRegistry()->RemoveSystem<UIImage>();
    _scene->GetEntitiesRegistry()->RemoveSystem<UIButton>();
    _scene->GetEntitiesRegistry()->RemoveSystem<UIInputField>();
    delete uiSystem;
}

void UILayer::LoadDefaultResources()
{
    auto resourcesManager = Application::Instance->GetResourcesManager();
    UIResources.DefaultInputFieldSprite = resourcesManager->LoadImage("debug_button.png", true);
    UIResources.DefaultInputFieldSprite->SetAsSliced(6);
    UIResources.DefaultPixelSprite = resourcesManager->LoadImage("pixel.png", true);
}

void UILayer::Update()
{
    if (_updateQueue.Size() == 0)
        return;

    for (int i = _updateQueue.Size() - 1; i >= 0; --i)
    {
        if (!_updateQueue[i].Callback(_updateQueue[i].Owner))
            _updateQueue.Remove(EntitiesRegistry::EntityIDGetID(_updateQueue[i].Owner));
    }
    _updateQueue.Condense();
}

void UILayer::Draw()
{
    // Prepare to draw
    auto entitiesRegistry = _scene->GetEntitiesRegistry();
    // TODO: support non default thickness
    _layerThickness = entitiesRegistry->GetComponentIterator<RectTransformation>().Size() + 1;

    // Update rect transformations if needed
    auto hierarchyNodes = entitiesRegistry->GetComponentIterator<HierarchyNode>();
    auto rtAccessor = entitiesRegistry->GetComponentAccessor<RectTransformation>();
    // Components to apply changed transformation
    auto imageAccessor = entitiesRegistry->GetComponentAccessor<UIImage>();
    auto textAccessor = entitiesRegistry->GetComponentAccessor<UIText>();
    for (auto& hierarchyNode : hierarchyNodes)
    {
        if (rtAccessor.Has(hierarchyNode.Owner))
        {
            auto& rt = rtAccessor.Get(hierarchyNode.Owner);
            rt.UpdateTransformation(this, rtAccessor, hierarchyNode);

            bool transformationDirty = rt.DidTransformationChange();
            if ((transformationDirty || _rebuildSortingOrder) && imageAccessor.Has(hierarchyNode.Owner))
                imageAccessor.Get(hierarchyNode.Owner).UpdateRenderer(rt, transformationDirty, _rebuildSortingOrder);
            if (textAccessor.Has(hierarchyNode.Owner))
                // Possible place for optimization - entering component even if transformation not dirty
                textAccessor.Get(hierarchyNode.Owner).Rebuild(this, rt, transformationDirty, _rebuildSortingOrder);
        }
    }

    // Rebuild elements after rt update
    auto uiIFs = entitiesRegistry->GetComponentIterator<UIInputField>();
    for (auto& uiIF : uiIFs)
        uiIF.Rebuild(this, rtAccessor.Get(uiIF.Owner));

    // After rebuilding text we need to condense renderers list to not wait for the next frame
    entitiesRegistry->ClearRemoved<UIQuadRenderer>();

    // Sort all UI objects by SortingOrder
    struct
    {
        bool operator()(UIQuadRenderer& a, UIQuadRenderer& b) const
        { return a.SortingOrder < b.SortingOrder; }
    } SOComparer;
    entitiesRegistry->SortComponents<UIQuadRenderer>(SOComparer);

    // Draw
    auto uiRenderers = entitiesRegistry->GetComponentIterator<UIQuadRenderer>();

    Renderer::SetDrawMode(DrawModes::Normal);
    // Opaque pass
    for (int i = 0; i < uiRenderers.Size(); ++i)
        if (uiRenderers[i].Queue == RenderingQueue::Opaque)
            Renderer::Draw((const QuadRenderer&)uiRenderers[i]);
    Renderer::EndBatch();

    Renderer::StartBatch();
    Renderer::SetDrawMode(DrawModes::Normal);
    // Transparent pass
    for (int i = uiRenderers.Size() - 1; i >= 0; --i)
        if (uiRenderers[i].Queue == RenderingQueue::Transparent)
            Renderer::Draw((const QuadRenderer&)uiRenderers[i]);

    // Refresh rect transformation
    auto rectTransformations = entitiesRegistry->GetComponentIterator<RectTransformation>();
    for (auto& rt : rectTransformations)
        rt.RefreshTransformation();
    auto uiTexts = entitiesRegistry->GetComponentIterator<UIText>();
    for (auto& uiText : uiTexts)
        uiText.Refresh();
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
        { return a.SortingOrder > b.SortingOrder; }
    } SOComparer;
    entitiesRegistry->SortComponents<UIEventHandler>(SOComparer);

    int size = uiEventHandlers.Size();
    for (int i = 0; i < size; ++i)
        if (uiEventHandlers[i].IsAlive())
            uiEventHandlers[i].HandleEvent(rtAccessor.Get(uiEventHandlers[i].Owner), uiEvent);

    _isPointerOverUI = uiEvent.Used;
}

void UILayer::AddToUpdateQueue(EntityID entityID, UpdateIntaractable callback)
{
    auto id = EntitiesRegistry::EntityIDGetID(entityID);
    if (!_updateQueue.Has(id))
        _updateQueue.Add(id, callback, entityID);
}

void UILayer::RemoveFromUpdateQueue(EntityID entityID)
{
    auto id = EntitiesRegistry::EntityIDGetID(entityID);
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

EntityID UILayer::CreateUIElement(const char* name, EntityID parent)
{
    auto entity = _scene->CreateEmptyEntity();
    auto& nameComponent = _scene->GetEntitiesRegistry()->AddComponent<NameComponent>(entity);
    nameComponent.Name = name;
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

EntityID UILayer::CreateUIImage(Sprite* sprite, const char* name, EntityID parent)
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

EntityID UILayer::CreateUIButton(Sprite* sprite, const char* name, EntityID parent)
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

EntityID UILayer::CreateUIText(const char* text, const char* name, EntityID parent)
{
    auto entity = CreateUIElement(name, parent);
    _scene->GetEntitiesRegistry()->AddComponent<UIText>(entity).SetText(text);

    return entity;
}

EntityID UILayer::CreateUIInputField()
{
    return CreateUIInputField("Input field", NULL_ENTITY);
}

EntityID UILayer::CreateUIInputField(const char* name, EntityID parent)
{
    auto entity = CreateUIElement(name, parent);
    auto& uiImage = _scene->GetEntitiesRegistry()->AddComponent<UIImage>(entity);
    uiImage.SetImage(UIResources.DefaultInputFieldSprite);
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
