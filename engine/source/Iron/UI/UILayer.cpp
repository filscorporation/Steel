#include "UILayer.h"
#include "../Core/Application.h"
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
}

UILayer::~UILayer()
{
    _scene->GetEntitiesRegistry()->RemoveSystem<UIText>();
    _scene->GetEntitiesRegistry()->RemoveSystem<UIImage>();
    _scene->GetEntitiesRegistry()->RemoveSystem<UIButton>();
    delete uiSystem;
}

void UILayer::Update()
{
    if (_buttonsToUpdate.Size() == 0)
        return;

    auto entitiesRegistry = _scene->GetEntitiesRegistry();
    for (int i = _buttonsToUpdate.Size() - 1; i >= 0; --i)
    {
        auto& button = entitiesRegistry->GetComponent<UIButton>(entitiesRegistry->EntityActual(_buttonsToUpdate[i]));
        if (!button.UpdateTransition())
            _buttonsToUpdate.Remove(_buttonsToUpdate[i]);
    }
}

void UILayer::Draw()
{
    // Prepare to draw
    auto entitiesRegistry = _scene->GetEntitiesRegistry();

    // Update rect transformations if needed
    auto hierarchyNodes = entitiesRegistry->GetComponentIterator<HierarchyNode>();
    auto rtAccessor = entitiesRegistry->GetComponentAccessor<RectTransformation>();
    // Components to apply changed transformation
    auto imageAccessor = entitiesRegistry->GetComponentAccessor<UIImage>();
    auto buttonAccessor = entitiesRegistry->GetComponentAccessor<UIButton>();
    auto textAccessor = entitiesRegistry->GetComponentAccessor<UIText>();
    for (auto& hierarchyNode : hierarchyNodes)
    {
        if (rtAccessor.Has(hierarchyNode.Owner))
        {
            auto& rt = rtAccessor.Get(hierarchyNode.Owner);
            rt.UpdateTransformation(rtAccessor, hierarchyNode);

            bool transformationDirty = rt.DidTransformationChange();
            if (transformationDirty && imageAccessor.Has(hierarchyNode.Owner))
                imageAccessor.Get(hierarchyNode.Owner).UpdateRenderer(rt);
            if (textAccessor.Has(hierarchyNode.Owner))
                // Possible place for optimization - entering component even if transformation not dirty
                textAccessor.Get(hierarchyNode.Owner).Rebuild(rt, transformationDirty);
        }
    }

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
            uiRenderers[i].Render();
    Renderer::EndBatch();

    Renderer::StartBatch();
    Renderer::SetDrawMode(DrawModes::Text);
    // Text letters
    for (int i = 0; i < uiRenderers.Size(); ++i)
        if (uiRenderers[i].Queue == RenderingQueue::Text)
            uiRenderers[i].Render();
    Renderer::EndBatch();

    Renderer::StartBatch();
    Renderer::SetDrawMode(DrawModes::Normal);
    // Transparent pass
    for (int i = uiRenderers.Size() - 1; i >= 0; --i)
        if (uiRenderers[i].Queue == RenderingQueue::Transparent)
            uiRenderers[i].Render();

    // Refresh rect transformation
    auto rectTransformations = entitiesRegistry->GetComponentIterator<RectTransformation>();
    for (auto& rt : rectTransformations)
        rt.RefreshTransformation();
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
            uiEventHandlers[i].HandleEvent(rtAccessor.Get(uiEventHandlers[i].Owner), uiEvent);

    _isPointerOverUI = uiEvent.Used;
}

void UILayer::AddButtonToUpdateQueue(EntityID buttonID)
{
    auto id = EntitiesRegistry::EntityIDGetID(buttonID);
    if (!_buttonsToUpdate.Has(id))
        _buttonsToUpdate.Add(id);
}

void UILayer::RemoveButtonFromUpdateQueue(EntityID buttonID)
{
    auto id = EntitiesRegistry::EntityIDGetID(buttonID);
    if (_buttonsToUpdate.Has(id))
        _buttonsToUpdate.Remove(id);
}

bool UILayer::IsPointerOverUI()
{
    return _isPointerOverUI;
}

UILayer* UILayer::Current()
{
    return Application::Instance->GetCurrentScene()->GetUILayer();
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
