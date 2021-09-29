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

    parentEntity = layer->CreateUIImage(layer->UIResources.DefaultFrameSprite, "Nodes", Owner);
    auto& nodesParentRT = entitiesRegistry->GetComponent<RectTransformation>(parentEntity);
    nodesParentRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    nodesParentRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
}

void HierarchyView::Update(EntitiesRegistry* entitiesRegistry)
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
    auto editor = (EditorApplication*)Application::Instance;
    auto appScene = editor->GetAppContext()->Scenes->GetActiveScene();
    auto sceneRegistry = appScene->GetEntitiesRegistry();

    auto nodes = new std::vector<HierarchyViewNode>();
    GetNodesData(sceneRegistry, (HierarchyParent&)*appScene, nodes);

    // TODO: temp
    if (lastNodes != nullptr)
    {
        for (auto& node : *lastNodes)
        {
            entitiesRegistry->DeleteEntity(node.UIElementEntity);
        }
    }
    delete lastNodes;
    lastNodes = nullptr;

    int i = 0;
    for (auto& node : *nodes)
    {
        node.UIElementEntity = CreateNodeUIElement(entitiesRegistry, sceneRegistry, layer, parentEntity, node, i);
        i ++;
    }

    lastNodes = nodes;
}

void HierarchyView::OnRemoved(EntitiesRegistry* entitiesRegistry)
{
    delete lastNodes;
    lastNodes = nullptr;
}

void HierarchyView::GetNodesData(EntitiesRegistry* sceneRegistry, HierarchyParent& parent, std::vector<HierarchyViewNode>* nodes)
{
    if (parent.ChildrenCount == 0)
        return;

    EntityID currentNodeID = parent.FirstChildNode;
    uint32_t childrenCount = parent.ChildrenCount;
    for (uint32_t i = 0; i < childrenCount; ++i)
    {
        auto& currentChildNode = sceneRegistry->GetComponent<HierarchyNode>(currentNodeID);

        nodes->emplace_back();
        (*nodes)[nodes->size() - 1].NodeEntity = currentNodeID;

        // Recursively call for children
        GetNodesData(sceneRegistry, (HierarchyParent&)currentChildNode, nodes);

        currentNodeID = currentChildNode.NextNode;
    }
}

EntityID HierarchyView::CreateNodeUIElement(EntitiesRegistry* entitiesRegistry, EntitiesRegistry* sceneRegistry,
                                            UILayer* layer, EntityID parentEntity, const HierarchyViewNode& node, int index)
{
    auto& nodeName = sceneRegistry->GetComponent<NameComponent>(node.NodeEntity);
    auto& nodeHN = sceneRegistry->GetComponent<HierarchyNode>(node.NodeEntity);

    EntityID buttonEntity = layer->CreateUIButton(layer->UIResources.DefaultFrameSprite, "Node", parentEntity);
    auto& button = entitiesRegistry->GetComponent<UIButton>(buttonEntity);
    auto& buttonRT = entitiesRegistry->GetComponent<RectTransformation>(buttonEntity);
    buttonRT.SetParallelHierarchy(true);
    buttonRT.SetAnchorMin(glm::vec2(0.0f, 1.0f));
    buttonRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    buttonRT.SetOffsetMin(glm::vec2(OFFSET + STYLE_BUTTON_H * nodeHN.HierarchyDepth, 0.0f));
    buttonRT.SetOffsetMax(glm::vec2(OFFSET, 0.0f));
    buttonRT.SetSize(glm::vec2(0.0f, STYLE_BUTTON_H));
    buttonRT.SetPivot(glm::vec2(0.0f, 1.0f));
    buttonRT.SetAnchoredPosition(glm::vec2(0.0f, -OFFSET - STYLE_BUTTON_H * index));

    EntityID textEntity = layer->CreateUIText(nodeName.Name, "Node", buttonEntity);
    auto& text = entitiesRegistry->GetComponent<UIText>(textEntity);
    text.SetColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    auto& textRT = entitiesRegistry->GetComponent<RectTransformation>(textEntity);
    textRT.SetParallelHierarchy(true);
    textRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    textRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
    textRT.SetOffsetMin(glm::vec2(8.0f, 2.0f));
    textRT.SetOffsetMax(glm::vec2(8.0f, 2.0f));

    return buttonEntity;
}
