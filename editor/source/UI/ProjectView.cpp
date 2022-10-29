#include "ProjectView.h"
#include "UIEditorTab.h"
#include "../EditorCore/EditorBuilder.h"
#include "ProjectViewElement.h"

#include <Steel.h>

void ProjectView::RegisterType()
{
    REGISTER_COMPONENT(ProjectView);
}

void ProjectView::Init(EntitiesRegistry* entitiesRegistry)
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();

    EntityID frameEntity = layer->CreateUIImage(layer->UIResources.StraightFrameSprite, "Frame", Owner);
    auto& frameRT = entitiesRegistry->GetComponent<RectTransformation>(frameEntity);
    frameRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
    frameRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));

    targetPathString = std::string(Application::Context()->Resources->GetResourcesPath());

    {
        parentEntity = layer->CreateUIElement("Nodes", frameEntity);
        auto& nodesParentRT = entitiesRegistry->GetComponent<RectTransformation>(parentEntity);
        nodesParentRT.SetParallelHierarchy(true);
        nodesParentRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        nodesParentRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        nodesParentRT.SetOffsetMin(glm::vec2(STYLE_OFFSET, STYLE_OFFSET));
        nodesParentRT.SetOffsetMax(glm::vec2(STYLE_OFFSET, STYLE_OFFSET + STYLE_BUTTON_H * 1.2f));
    }
}

void ProjectView::Update(EntitiesRegistry* entitiesRegistry)
{
    EntityID parentEntity = entitiesRegistry->GetComponent<HierarchyNode>(Owner).GetParentNode();
    bool isFocused = entitiesRegistry->GetComponent<UIEditorTab>(parentEntity).GetIsFocused();

    std::filesystem::path currentPath(currentPathString);
    std::filesystem::path targetPath(targetPathString);

    if (isFocused && Input::IsKeyJustPressed(KeyCodes::Backspace))
    {
        if (!std::filesystem::equivalent(currentPath, std::filesystem::path(Application::Context()->Resources->GetResourcesPath())))
        {
            targetPath = currentPath.parent_path();
        }
    }

    if (currentPath != targetPath)
    {
        currentPathString = currentPath.u8string();
        targetPathString = targetPath.u8string();

        RebuildAll(entitiesRegistry);
    }
}

void ProjectView::FocusOnFile(EntitiesRegistry* entitiesRegistry, const std::string& filePath)
{
    std::filesystem::path currentPath(currentPathString);
    if (currentPath != std::filesystem::path(Application::Context()->Resources->GetResourcesPath()))
    {
        targetPathString = currentPath.parent_path();
    }
}

void ProjectView::Clear(EntitiesRegistry* entitiesRegistry)
{
    if (nodes != nullptr)
    {
        for (auto& node : *nodes)
        {
            entitiesRegistry->DeleteEntity(node.second.UIElementEntity);
        }
    }

    delete nodes;
    nodes = nullptr;
}

void ProjectView::RebuildAll(EntitiesRegistry* entitiesRegistry)
{
    Clear(entitiesRegistry);

    currentPathString = targetPathString;

    if (!std::filesystem::exists(currentPathString))
    {
        Log::LogError("Wrong path to display: \"{0}\"", currentPathString);
        return;
    }

    nodes = new std::unordered_map<std::string, ProjectViewNode>();
    for (std::filesystem::directory_iterator i(currentPathString), end; i != end; ++i)
    {
        (*nodes)[i->path().filename().u8string()] = CreateElement(entitiesRegistry, i->path(), (int)(*nodes).size());
    }
}

ProjectViewNode ProjectView::CreateElement(EntitiesRegistry* entitiesRegistry, const std::filesystem::path& filePath, int order)
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();

    EntityID elementEntity = layer->CreateUIElement("Node", parentEntity);

    ProjectViewNode projectViewNode;
    projectViewNode.Order = order;
    projectViewNode.NodeDirty = false;
    projectViewNode.FileName = filePath.filename().u8string();
    projectViewNode.PathString = filePath.u8string();
    projectViewNode.IsDirectory = is_directory(filePath);
    projectViewNode.UIElementEntity = elementEntity;

    entitiesRegistry->AddComponent<ProjectViewElement>(elementEntity).Init(entitiesRegistry, layer, (*this), projectViewNode);

    return projectViewNode;
}

void ProjectView::ElementClicked(EntityID elementID)
{
    if (nodes != nullptr)
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();

        for (auto& node : *nodes)
        {
            if (node.second.UIElementEntity == elementID)
            {
                // TODO: change to selection
                if (node.second.IsDirectory)
                    targetPathString = node.second.PathString;
            }
            // TODO: disselect
        }
    }
}
