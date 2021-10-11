#pragma once

#include <Steel/UI/UIComponent.h>
#include <Steel/UI/UILayer.h>
#include <Steel/EntityComponentSystem/EntitiesRegistry.h>

namespace NodeFlags
{
    enum NodeFlag
    {
        Expanded = 1 << 0,
        Visible  = 1 << 1,
        Selected = 1 << 2,
    };

    inline NodeFlag operator|(NodeFlag a, NodeFlag b)
    {
        return static_cast<NodeFlag>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline NodeFlag operator&(NodeFlag a, NodeFlag b)
    {
        return static_cast<NodeFlag>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }
}

struct HierarchyViewNode
{
    NodeFlags::NodeFlag Flags = NodeFlags::Visible;
    int Order = 0;
    bool NodeDirty = false;
    EntityID UIElementEntity = NULL_ENTITY;
};

class HierarchyView : public UIComponent
{
public:
    explicit HierarchyView(EntityID ownerEntityID) : UIComponent(ownerEntityID) { };

    void OnRemoved(EntitiesRegistry* entitiesRegistry) override;

    void Init(EntitiesRegistry* entitiesRegistry);
    void Update(EntitiesRegistry* entitiesRegistry);

private:
    EntityID _parentEntity = NULL_ENTITY;
    std::unordered_map<EntityID, HierarchyViewNode>* lastNodes = nullptr;

    static void GetNodesData(EntitiesRegistry* sceneRegistry, HierarchyParent& parent, std::unordered_map<EntityID, HierarchyViewNode>* nodes);
    static EntityID CreateNodeUIElement(EntitiesRegistry* entitiesRegistry, EntitiesRegistry* sceneRegistry, UILayer* layer,
                                        EntityID parentEntity, EntityID nodeEntity, const HierarchyViewNode& node);
    static EntityID PositionNodeUIElement(EntitiesRegistry* entitiesRegistry, EntitiesRegistry* sceneRegistry,
                                        EntityID nodeEntity, const HierarchyViewNode& node);
};
