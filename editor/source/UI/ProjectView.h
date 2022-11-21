#pragma once

#include <Steel/UI/UIComponent.h>
#include <Steel/UI/UILayer.h>
#include <Steel/UI/UIElements/UIButton.h>
#include <Steel/EntityComponentSystem/EntitiesRegistry.h>

#include <filesystem>

struct ProjectViewNode
{
    int Order = 0;
    bool IsSelected = false;
    bool NodeDirty = false;
    std::string FileName;
    std::string PathString;
    bool IsDirectory = false;
    EntityID UIElementEntity = NULL_ENTITY;
};

class ProjectView : public UIComponent
{
    DEFINE_TYPE(ProjectView)

public:
    explicit ProjectView(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void Init(EntitiesRegistry* entitiesRegistry);
    void Update(EntitiesRegistry* entitiesRegistry);

    void GetSelectedFiles(EntitiesRegistry* entitiesRegistry, std::vector<std::string>& selectedFiles);
    void FocusOnFile(EntitiesRegistry* entitiesRegistry, const std::string& filePath);
    void Clear(EntitiesRegistry* entitiesRegistry);
    void DeselectAll(EntitiesRegistry* entitiesRegistry);
    ProjectViewNode CreateElement(EntitiesRegistry* entitiesRegistry, const std::filesystem::path& filePath, int order) const;

    void ElementClicked(EntityID elementID);

private:
    void RebuildAll(EntitiesRegistry* entitiesRegistry);
    bool TryDoubleClick(const ProjectViewNode& node);
    void OnElementSelected(EntitiesRegistry* entitiesRegistry);

    std::string currentPathString;
    std::string targetPathString;
    float lastClickTime = -1.0f;

    EntityID _parentEntity = NULL_ENTITY;
    std::unordered_map<std::string, ProjectViewNode>* nodes = nullptr;
    bool _lockDeselectAll = false;
};
