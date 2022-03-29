#include "HierarchyView.h"
#include "HierarchyElement.h"
#include "../EditorCore/EditorApplication.h"
#include "../EditorCore/EditorBuilder.h"
#include "UIEditorTab.h"
#include "SceneView.h"

#include <Steel.h>

void HierarchyView::RegisterType()
{
    REGISTER_COMPONENT(HierarchyView);
}

void HierarchyView::Init(EntitiesRegistry* entitiesRegistry)
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();

    EntityID frameEntity = layer->CreateUIImage(layer->UIResources.StraightFrameSprite, "Frame", Owner);
    auto& frameRT = entitiesRegistry->GetComponent<RectTransformation>(frameEntity);
    frameRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    frameRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));

    {
        EntityID tabEntity = layer->CreateUIImage(layer->UIResources.DefaultPixelSprite, "Tab image", frameEntity);
        auto& tabImage = entitiesRegistry->GetComponent<UIImage>(tabEntity);
        tabImage.SetColor(STYLE_DARK_GREY);
        auto& tabImageRT = entitiesRegistry->GetComponent<RectTransformation>(tabEntity);
        tabImageRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
        tabImageRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        tabImageRT.SetOffsetMin(glm::vec2(STYLE_OFFSET, 0.0f));
        tabImageRT.SetOffsetMax(glm::vec2(STYLE_OFFSET, 0.0f));
        tabImageRT.SetAnchoredPosition(glm::vec2(0.0f, -STYLE_BUTTON_H * 1.2f * 0.5f - STYLE_OFFSET));
        tabImageRT.SetSize(glm::vec2(0.0f, STYLE_BUTTON_H * 1.2f));

        {
            EntityID createEntity = layer->CreateUIButton(layer->UIResources.DefaultFrameSprite, "Create button", tabEntity);
            auto& buttonRT = entitiesRegistry->GetComponent<RectTransformation>(createEntity);
            buttonRT.SetSize(glm::vec2(STYLE_BUTTON_H * 3.0f, STYLE_BUTTON_H));
            buttonRT.SetPivot(glm::vec2(0.0f, 0.5f));
            buttonRT.SetAnchorMin(glm::vec2(0.0f, 0.5f));
            buttonRT.SetAnchorMax(glm::vec2(0.0f, 0.5f));
            buttonRT.SetAnchoredPosition(glm::vec2(STYLE_OFFSET, 0.0f));
            EntityID textEntity = layer->CreateUIText("Create", "Text", createEntity);
            auto& text = entitiesRegistry->GetComponent<UIText>(textEntity);
            text.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            auto& textRT = entitiesRegistry->GetComponent<RectTransformation>(textEntity);
            textRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
            textRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
            textRT.SetOffsetMin(glm::vec2(8, 2));
            textRT.SetOffsetMax(glm::vec2(8, 2));

            auto& button = entitiesRegistry->GetComponent<UIButton>(createEntity);
            EntityID hierarchyViewEntity = Owner;
            button.Callback = [hierarchyViewEntity](EntityID entityID)
            {
                auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
                registry->GetComponent<HierarchyView>(hierarchyViewEntity).CreateNewEntityInHierarchy();
            };
        }
    }

    {
        _parentEntity = layer->CreateUIElement("Nodes", frameEntity);
        auto& nodesParentRT = entitiesRegistry->GetComponent<RectTransformation>(_parentEntity);
        nodesParentRT.SetParallelHierarchy(true);
        nodesParentRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        nodesParentRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        nodesParentRT.SetOffsetMin(glm::vec2(STYLE_OFFSET, STYLE_OFFSET));
        nodesParentRT.SetOffsetMax(glm::vec2(STYLE_OFFSET, STYLE_OFFSET + STYLE_BUTTON_H * 1.2f));
    }
}

void HierarchyView::Update(EntitiesRegistry* entitiesRegistry)
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
    auto editor = (EditorApplication*)Application::Instance;
    auto appScene = editor->GetAppContext()->Scenes->GetActiveScene();
    auto sceneRegistry = appScene->GetEntitiesRegistry();

    EntityID parentEntity = entitiesRegistry->GetComponent<HierarchyNode>(Owner).GetParentNode();
    bool isFocused = entitiesRegistry->GetComponent<UIEditorTab>(parentEntity).GetIsFocused();

    auto nodes = new std::unordered_map<EntityID, HierarchyViewNode>();
    GetNodesData(sceneRegistry, (HierarchyParent&)*appScene, nodes);

    if (lastNodes != nullptr)
    {
        for (auto& node : *lastNodes)
        {
            if (nodes->find(node.first) == nodes->end() || (*nodes)[node.first].NodeDirty)
                entitiesRegistry->DeleteEntity(node.second.UIElementEntity);
        }
    }

    bool deselect = isFocused && Input::IsKeyJustPressed(KeyCodes::Escape);

    for (auto& node : *nodes)
    {
        if (lastNodes == nullptr || node.second.NodeDirty || lastNodes->find(node.first) == lastNodes->end())
        {
            node.second.UIElementEntity = CreateNodeUIElement(entitiesRegistry, sceneRegistry, layer, _parentEntity, node.first, node.second);
        }
        else
        {
            node.second.UIElementEntity = (*lastNodes)[node.first].UIElementEntity;
            auto& hierarchyElement = entitiesRegistry->GetComponent<HierarchyElement>(node.second.UIElementEntity);
            hierarchyElement.UpdatePosition(entitiesRegistry, sceneRegistry, node.first, node.second);

            if (deselect && node.second.Flags & NodeFlags::Selected)
            {
                node.second.Flags = node.second.Flags & ~NodeFlags::Selected;
                hierarchyElement.SetDefaultNodeStyle(entitiesRegistry);
            }
        }
        node.second.NodeDirty = false;
    }

    delete lastNodes;
    lastNodes = nodes;

    // Controls
    if (isFocused && Input::IsKeyJustPressed(KeyCodes::Delete))
    {
        DeleteSelectedEntities();
    }
    if (isFocused && Input::IsKeyJustPressed(KeyCodes::F))
    {
        FocusOnSelectedEntities(entitiesRegistry);
    }
}

void HierarchyView::Clear(EntitiesRegistry* entitiesRegistry)
{
    if (lastNodes != nullptr)
    {
        for (auto& node : *lastNodes)
        {
            entitiesRegistry->DeleteEntity(node.second.UIElementEntity);
        }
    }

    delete lastNodes;
    lastNodes = nullptr;
}

void HierarchyView::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    delete lastNodes;
    lastNodes = nullptr;
}

void HierarchyView::GetNodesData(EntitiesRegistry* sceneRegistry, HierarchyParent& parent, std::unordered_map<EntityID, HierarchyViewNode>* nodes)
{
    if (parent.GetChildrenCount() == 0)
        return;

    EntityID currentNodeID = parent.GetFirstChildNode();
    uint32_t childrenCount = parent.GetChildrenCount();
    for (uint32_t i = 0; i < childrenCount; ++i)
    {
        auto& currentChildNode = sceneRegistry->GetComponent<HierarchyNode>(currentNodeID);
        bool recursive = true;

        (*nodes)[currentNodeID].Order = (int)(*nodes).size();
        (*nodes)[currentNodeID].NodeDirty = currentChildNode.IsDirty;
        (*nodes)[currentNodeID].HasChildren = currentChildNode.GetChildrenCount() != 0;

        if (lastNodes != nullptr && (*lastNodes).find(currentNodeID) != (*lastNodes).end())
        {
            (*nodes)[currentNodeID].Flags = (*lastNodes)[currentNodeID].Flags;

            if ((*lastNodes)[currentNodeID].NodeDirty)
                (*nodes)[currentNodeID].NodeDirty = true;
            if (!((*lastNodes)[currentNodeID].Flags & NodeFlags::Expanded))
                recursive = false;
        }

        if (sceneRegistry->EntityGetState(currentNodeID) & EntityStates::IsActive)
            (*nodes)[currentNodeID].Flags = (*nodes)[currentNodeID].Flags | NodeFlags::Active;
        else
            (*nodes)[currentNodeID].Flags = (*nodes)[currentNodeID].Flags & ~NodeFlags::Active;

        if (recursive)
        {
            // Recursively call for children
            GetNodesData(sceneRegistry, (HierarchyParent&)currentChildNode, nodes);
        }

        currentNodeID = currentChildNode.GetNextNode();
    }
}

EntityID HierarchyView::CreateNodeUIElement(EntitiesRegistry* entitiesRegistry, EntitiesRegistry* sceneRegistry,
                                            UILayer* layer, EntityID parentEntity, EntityID nodeEntity, const HierarchyViewNode& node)
{
    EntityID elementEntity = layer->CreateUIElement("Node", parentEntity);
    entitiesRegistry->AddComponent<HierarchyElement>(elementEntity).Init(entitiesRegistry, sceneRegistry, layer, (*this), nodeEntity, node);

    return elementEntity;
}

void HierarchyView::ElementClicked(EntityID elementID)
{
    if (lastNodes != nullptr)
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        bool additiveSelect = Input::IsKeyPressed(KeyCodes::LeftControl) || Input::IsKeyPressed(KeyCodes::RightControl);

        for (auto& node : *lastNodes)
        {
            if (node.second.UIElementEntity == elementID)
            {
                if (!(node.second.Flags & NodeFlags::Selected))
                {
                    node.second.Flags = node.second.Flags | NodeFlags::Selected;
                    registry->GetComponent<HierarchyElement>(node.second.UIElementEntity).SetSelectedNodeStyle(registry);
                }
            }
            else
            {
                if (!additiveSelect && node.second.Flags & NodeFlags::Selected)
                {
                    // Deselect all others nodes
                    node.second.Flags = node.second.Flags & ~NodeFlags::Selected;
                    registry->GetComponent<HierarchyElement>(node.second.UIElementEntity).SetDefaultNodeStyle(registry);
                }
            }
        }
    }
}

void HierarchyView::ElementExpanded(EntityID elementID)
{
    if (lastNodes != nullptr)
    {
        for (auto& node : *lastNodes)
        {
            if (node.second.UIElementEntity == elementID)
            {
                if (node.second.Flags & NodeFlags::Expanded)
                    node.second.Flags = node.second.Flags & ~NodeFlags::Expanded;
                else
                    node.second.Flags = node.second.Flags | NodeFlags::Expanded;
                node.second.NodeDirty = true;
            }
        }
    }
}

void HierarchyView::GetSelectedEntities(EntitiesRegistry* entitiesRegistry, std::vector<EntityID>& selectedEntities)
{
    for (auto& node : *lastNodes)
    {
        if (node.second.Flags & NodeFlags::Selected)
        {
            selectedEntities.push_back(node.first);
        }
    }
}

void HierarchyView::FocusOnSelectedEntities(EntitiesRegistry* entitiesRegistry)
{
    std::vector<EntityID> selectedEntities;
    GetSelectedEntities(entitiesRegistry, selectedEntities);

    if (!selectedEntities.empty())
    {
        auto sceneViewIterator = entitiesRegistry->GetComponentIterator<SceneView>();
        for (int i = 0; i < sceneViewIterator.Size(); ++i)
        {
            if (sceneViewIterator[i].IsAlive())
                sceneViewIterator[i].FocusCameraOnEntity(entitiesRegistry, selectedEntities);
        }
    }
}

void HierarchyView::DeleteSelectedEntities()
{
    auto editor = (EditorApplication*)Application::Instance;
    auto appScene = editor->GetAppContext()->Scenes->GetActiveScene();

    editor->SwitchContext(editor->AppContext);
    for (auto& node : *lastNodes)
    {
        if (node.second.Flags & NodeFlags::Selected)
        {
            appScene->DestroyEntity(node.first);
        }
    }
    editor->SwitchContext(editor->EditorContext);
}

void HierarchyView::CreateNewEntityInHierarchy()
{
    auto editor = (EditorApplication*)Application::Instance;
    auto appScene = editor->GetAppContext()->Scenes->GetActiveScene();
    bool parentFound = false;

    editor->SwitchContext(editor->AppContext);
    if (lastNodes != nullptr)
    {
        for (auto& node : *lastNodes)
        {
            if (node.second.Flags & NodeFlags::Selected)
            {
                appScene->CreateEntity("New entity", node.first);
                parentFound = true;
            }
        }
    }
    if (!parentFound)
    {
        appScene->CreateEntity("New entity", NULL_ENTITY);
    }
    editor->SwitchContext(editor->EditorContext);
}
