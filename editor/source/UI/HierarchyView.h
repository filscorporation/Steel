#pragma once

#include <Steel/UI/UIComponent.h>
#include <Steel/UI/UILayer.h>
#include <Steel/UI/UIElements/UIButton.h>
#include <Steel/EntityComponentSystem/EntitiesRegistry.h>

namespace NodeFlags
{
    enum NodeFlag
    {
        Expanded = 1 << 0,
        Visible  = 1 << 1,
        Selected = 1 << 2,
        Active   = 1 << 3,
    };

    inline NodeFlag operator|(NodeFlag a, NodeFlag b)
    {
        return static_cast<NodeFlag>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline NodeFlag operator&(NodeFlag a, NodeFlag b)
    {
        return static_cast<NodeFlag>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }

    inline NodeFlag operator~(NodeFlag a)
    {
        return static_cast<NodeFlag>(~static_cast<uint32_t>(a));
    }
}

struct HierarchyViewNode
{
    NodeFlags::NodeFlag Flags = NodeFlags::Expanded | NodeFlags::Visible;
    int Order = 0;
    bool NodeDirty = false;
    bool HasChildren = false;
    EntityID UIElementEntity = NULL_ENTITY;
};

class HierarchyView : public UIComponent
{
    DEFINE_TYPE(HierarchyView)

public:
    explicit HierarchyView(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void Init(EntitiesRegistry* entitiesRegistry);
    void Update(EntitiesRegistry* entitiesRegistry);

    void ElementClicked(EntityID elementID);
    void ElementExpanded(EntityID elementID);

    void GetSelectedEntities(EntitiesRegistry* entitiesRegistry, std::vector<EntityID>& selectedEntities);
    void FocusOnSelectedEntities(EntitiesRegistry* entitiesRegistry);
    void DeselectAll(EntitiesRegistry* entitiesRegistry);

private:
    void GetNodesData(EntitiesRegistry* sceneRegistry, HierarchyParent& parent, std::unordered_map<EntityID, HierarchyViewNode>* nodes);
    EntityID CreateNodeUIElement(EntitiesRegistry* entitiesRegistry, EntitiesRegistry* sceneRegistry, UILayer* layer,
                                        EntityID parentEntity, EntityID nodeEntity, const HierarchyViewNode& node);

    void Clear(EntitiesRegistry* entitiesRegistry);
    void DeleteSelectedEntities();
    void CreateNewEntityInHierarchy();
    void OnElementSelected(EntitiesRegistry* entitiesRegistry);

    EntityID _parentEntity = NULL_ENTITY;
    Scene* lastScene = nullptr;
    std::unordered_map<EntityID, HierarchyViewNode>* lastNodes = nullptr;
    bool _lockDeselectAll = false;
};
