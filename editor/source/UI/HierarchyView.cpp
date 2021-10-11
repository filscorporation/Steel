#include "HierarchyView.h"
#include "../EditorCore/EditorApplication.h"
#include "../EditorCore/EditorBuilder.h"

#include <Steel.h>
#include <Steel/Scene/NameComponent.h>
#include <Steel/Scene/HierarchyNode.h>

#define OFFSET 3

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

    for (auto& node : *nodes)
    {
        if (lastNodes == nullptr || node.second.NodeDirty)
            node.second.UIElementEntity = CreateNodeUIElement(entitiesRegistry, sceneRegistry, layer, _parentEntity, node.first, node.second);
        else
        {
            node.second.UIElementEntity = (*lastNodes)[node.first].UIElementEntity;
            PositionNodeUIElement(entitiesRegistry, sceneRegistry, node.first, node.second);
        }
    }

    delete lastNodes;
    lastNodes = nodes;
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

        (*nodes)[currentNodeID].Order = (int)(*nodes).size();
        (*nodes)[currentNodeID].NodeDirty = currentChildNode.IsDirty;

        // Recursively call for children
        GetNodesData(sceneRegistry, (HierarchyParent&)currentChildNode, nodes);

        currentNodeID = currentChildNode.NextNode;
    }
}

EntityID HierarchyView::CreateNodeUIElement(EntitiesRegistry* entitiesRegistry, EntitiesRegistry* sceneRegistry,
                                            UILayer* layer, EntityID parentEntity, EntityID nodeEntity, const HierarchyViewNode& node)
{
    auto& nodeName = sceneRegistry->GetComponent<NameComponent>(nodeEntity);
    auto& nodeHN = sceneRegistry->GetComponent<HierarchyNode>(nodeEntity);

    EntityID buttonEntity = layer->CreateUIButton(layer->UIResources.DefaultFrameSprite, "Node", parentEntity);
    auto& button = entitiesRegistry->GetComponent<UIButton>(buttonEntity);
    auto& buttonRT = entitiesRegistry->GetComponent<RectTransformation>(buttonEntity);
    buttonRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
    buttonRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    buttonRT.SetOffsetMin(glm::vec2(OFFSET + STYLE_BUTTON_H * nodeHN.HierarchyDepth, 0.0f));
    buttonRT.SetOffsetMax(glm::vec2(OFFSET, 0.0f));
    buttonRT.SetSize(glm::vec2(0.0f, STYLE_BUTTON_H));
    buttonRT.SetPivot(glm::vec2(0.0f, 1.0f));
    buttonRT.SetAnchoredPosition(glm::vec2(0.0f, -OFFSET - STYLE_BUTTON_H * node.Order));

    EntityID textEntity = layer->CreateUIText(nodeName.Name, "Node", buttonEntity);
    auto& text = entitiesRegistry->GetComponent<UIText>(textEntity);
    text.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    auto& textRT = entitiesRegistry->GetComponent<RectTransformation>(textEntity);
    textRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    textRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    textRT.SetOffsetMin(glm::vec2(8.0f, 2.0f));
    textRT.SetOffsetMax(glm::vec2(8.0f, 2.0f));

    return buttonEntity;
}

EntityID HierarchyView::PositionNodeUIElement(EntitiesRegistry* entitiesRegistry, EntitiesRegistry* sceneRegistry,
                                              EntityID nodeEntity, const HierarchyViewNode& node)
{
    auto& nodeHN = sceneRegistry->GetComponent<HierarchyNode>(nodeEntity);

    auto& button = entitiesRegistry->GetComponent<UIButton>(node.UIElementEntity);
    auto& buttonRT = entitiesRegistry->GetComponent<RectTransformation>(node.UIElementEntity);
    buttonRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
    buttonRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    buttonRT.SetOffsetMin(glm::vec2(OFFSET + STYLE_BUTTON_H * nodeHN.HierarchyDepth, 0.0f));
    buttonRT.SetOffsetMax(glm::vec2(OFFSET, 0.0f));
    buttonRT.SetSize(glm::vec2(0.0f, STYLE_BUTTON_H));
    buttonRT.SetPivot(glm::vec2(0.0f, 1.0f));
    buttonRT.SetAnchoredPosition(glm::vec2(0.0f, -OFFSET - STYLE_BUTTON_H * node.Order));

    return node.UIElementEntity;
}
