#pragma once

#include <Steel/UI/UIComponent.h>
#include <Steel/UI/UILayer.h>
#include <Steel/UI/UIElements/UIButton.h>
#include <Steel/EntityComponentSystem/EntitiesRegistry.h>

#include <filesystem>

struct ProjectViewNode
{
    int Order = 0;
    bool NodeDirty = false;
    std::string FileName;
    std::string PathString;
    bool IsDirectory = false;
    EntityID UIElementEntity = NULL_ENTITY;
};

class ProjectView : public UIComponent
{
    DEFINE_COMPONENT(ProjectView)

public:
    explicit ProjectView(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void Init(EntitiesRegistry* entitiesRegistry);
    void Update(EntitiesRegistry* entitiesRegistry);

    void FocusOnFile(EntitiesRegistry* entitiesRegistry, const std::string& filePath);
    void Clear(EntitiesRegistry* entitiesRegistry);
    ProjectViewNode CreateElement(EntitiesRegistry* entitiesRegistry, const std::filesystem::path& filePath, int order);

    void ElementClicked(EntityID elementID);

private:
    std::string currentPathString;
    std::string targetPathString;

    EntityID _parentEntity = NULL_ENTITY;
    std::unordered_map<std::string, ProjectViewNode>* nodes = nullptr;

    void RebuildAll(EntitiesRegistry* entitiesRegistry);
};
