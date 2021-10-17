#include "HierarchyView.h"
#include "HierarchyElement.h"
#include "../EditorCore/EditorApplication.h"

#include <Steel.h>

void HierarchyView::Init(EntitiesRegistry* entitiesRegistry)
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();

    _parentEntity = layer->CreateUIImage(layer->UIResources.DefaultFrameSprite, "Nodes", Owner);
    auto& nodesParentRT = entitiesRegistry->GetComponent<RectTransformation>(_parentEntity);
    nodesParentRT.SetParallelHierarchy(true);
    nodesParentRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    nodesParentRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
}

void HierarchyView::Update(EntitiesRegistry* entitiesRegistry)
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
    auto editor = (EditorApplication*)Application::Instance;
    auto appScene = editor->GetAppContext()->Scenes->GetActiveScene();
    auto sceneRegistry = appScene->GetEntitiesRegistry();

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

    bool deselect = Input::IsKeyJustPressed(KeyCodes::Escape);

    for (auto& node : *nodes)
    {
        if (lastNodes == nullptr || node.second.NodeDirty || lastNodes->find(node.first) == lastNodes->end())
        {
            node.second.UIElementEntity = CreateNodeUIElement(entitiesRegistry, sceneRegistry, layer, _parentEntity, node.first, node.second);
        }
        else
        {
            if (lastNodes->find(node.first) == lastNodes->end())
            {
                int a = 0;
            }
            node.second.UIElementEntity = (*lastNodes)[node.first].UIElementEntity;
            node.second.Flags = (*lastNodes)[node.first].Flags;
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

    if (Input::IsKeyJustPressed(KeyCodes::Delete))
    {
        editor->SwitchContext(editor->AppContext);
        for (auto& node : *nodes)
        {
            if (node.second.Flags & NodeFlags::Selected)
            {
                appScene->DestroyEntity(node.first);
            }
        }
        editor->SwitchContext(editor->EditorContext);
    }
}

void HierarchyView::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    delete lastNodes;
    lastNodes = nullptr;
}

void HierarchyView::GetNodesData(EntitiesRegistry* sceneRegistry, HierarchyParent& parent, std::unordered_map<EntityID, HierarchyViewNode>* nodes)
{
    if (parent.ChildrenCount == 0)
        return;

    EntityID currentNodeID = parent.FirstChildNode;
    uint32_t childrenCount = parent.ChildrenCount;
    for (uint32_t i = 0; i < childrenCount; ++i)
    {
        auto& currentChildNode = sceneRegistry->GetComponent<HierarchyNode>(currentNodeID);
        bool recursive = true;

        (*nodes)[currentNodeID].Order = (int)(*nodes).size();
        (*nodes)[currentNodeID].NodeDirty = currentChildNode.IsDirty;
        (*nodes)[currentNodeID].HasChildren = currentChildNode.ChildrenCount != 0;

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

        currentNodeID = currentChildNode.NextNode;
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
    bool additiveSelect = Input::IsKeyPressed(KeyCodes::LeftControl) || Input::IsKeyPressed(KeyCodes::RightControl);

    if (lastNodes != nullptr)
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

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
