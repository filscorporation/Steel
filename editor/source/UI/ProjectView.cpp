#include "ProjectView.h"
#include "UIEditorTab.h"
#include "ProjectViewElement.h"
#include "../EditorCore/EditorBuilder.h"

#include <Steel.h>

#define MIN_DOUBLE_CLICK_TIME 0.4f

void ProjectView::RegisterType()
{
    REGISTER_COMPONENT(ProjectView);
}

void ProjectView::Init(EntitiesRegistry* entitiesRegistry)
{
    targetPathString = std::string(Application::Context()->Resources->GetResourcesPath());

    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();
    {
        _parentEntity = layer->CreateUIElement("Nodes", Owner);
        auto& nodesParentRT = entitiesRegistry->GetComponent<RectTransformation>(_parentEntity);
        nodesParentRT.SetParallelHierarchy(true);
        nodesParentRT.SetAnchorMin(glm::vec2(0.0f, 0.0f));
        nodesParentRT.SetAnchorMax(glm::vec2(1.0f, 1.0f));
        nodesParentRT.SetOffsetMin(glm::vec2(STYLE_OFFSET, STYLE_OFFSET));
        nodesParentRT.SetOffsetMax(glm::vec2(STYLE_OFFSET, STYLE_OFFSET + STYLE_BUTTON_H * 1.2f));
    }
}

void ProjectView::Update(EntitiesRegistry* entitiesRegistry)
{
    // Parent of a parent (scrollable view and then editor tab)
    EntityID parentEntity = entitiesRegistry->GetComponent<HierarchyNode>(Owner).GetParentNode();
    parentEntity = entitiesRegistry->GetComponent<HierarchyNode>(parentEntity).GetParentNode();
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

void ProjectView::GetSelectedFiles(EntitiesRegistry* entitiesRegistry, std::vector<std::string>& selectedFiles)
{
    for (auto& node : *nodes)
    {
        if (node.second.IsSelected)
        {
            selectedFiles.push_back(node.second.PathString);
        }
    }
}

void ProjectView::FocusOnFile(EntitiesRegistry* entitiesRegistry, const std::string& filePath)
{
    std::filesystem::path currentPath(currentPathString);
    if (currentPath != std::filesystem::path(Application::Context()->Resources->GetResourcesPath()))
    {
        targetPathString = currentPath.parent_path().u8string();
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

    auto& rt = entitiesRegistry->GetComponent<RectTransformation>(Owner);
    rt.SetAnchoredPosition(glm::vec2(0.0f, 0.0f));
    rt.SetSize(glm::vec2(0.0f, (float)(*nodes).size() * STYLE_BUTTON_H + STYLE_OFFSET * 2 + STYLE_BUTTON_H * 1.2f));
}

ProjectViewNode ProjectView::CreateElement(EntitiesRegistry* entitiesRegistry, const std::filesystem::path& filePath, int order) const
{
    auto layer = Application::Context()->Scenes->GetActiveScene()->GetUILayer();

    EntityID elementEntity = layer->CreateUIElement("Node", _parentEntity);

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
    auto entitiesRegistry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
    auto& rt = entitiesRegistry->GetComponent<RectTransformation>(Owner);

    if (nodes != nullptr)
    {
        auto registry = Application::Instance->GetCurrentScene()->GetEntitiesRegistry();
        bool additiveSelect = Input::IsKeyPressed(KeyCodes::LeftControl) || Input::IsKeyPressed(KeyCodes::RightControl);

        for (auto& node : *nodes)
        {
            if (node.second.UIElementEntity == elementID)
            {
                if (!node.second.IsSelected)
                {
                    node.second.IsSelected = true;
                    registry->GetComponent<ProjectViewElement>(node.second.UIElementEntity).SetSelectedNodeStyle(registry);
                }

                if (TryDoubleClick(node.second))
                    break;
            }
            else
            {
                if (!additiveSelect && node.second.IsSelected)
                {
                    // Deselect all others nodes
                    node.second.IsSelected = false;
                    registry->GetComponent<ProjectViewElement>(node.second.UIElementEntity).SetDefaultNodeStyle(registry);
                }
            }
        }
    }
}

bool ProjectView::TryDoubleClick(const ProjectViewNode& node)
{
    bool result = false;
    float currentClickTime = Time::TimeSinceStartup();
    if (lastClickTime > 0.0f && currentClickTime - lastClickTime <= MIN_DOUBLE_CLICK_TIME)
    {
        if (node.IsDirectory)
        {
            targetPathString = node.PathString;
            result = true;
        }
    }
    lastClickTime = currentClickTime;

    return result;
}
