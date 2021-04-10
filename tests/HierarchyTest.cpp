#include "TestsSetUp.h"

#include "Iron.h"
#include "Iron/Scene/Hierarchy.h"

TEST(HierarchyTest, Creation)
{
    EntitiesRegistry registry;
    HierarchySystem system;
    registry.RegisterSystem(&system);

    EntityID entityID = registry.CreateNewEntity();
    registry.AddComponent<HierarchyNode>(entityID);

    auto children = GetAllChildren(&registry, entityID);
    EXPECT_EQ(children.size(), 0);
}

TEST(HierarchyTest, SimpleAdd)
{
    EntitiesRegistry registry;
    HierarchySystem system;
    registry.RegisterSystem(&system);

    EntityID entityID = registry.CreateNewEntity();
    registry.AddComponent<HierarchyNode>(entityID);
    EntityID entityID2 = registry.CreateNewEntity();
    registry.AddComponent<HierarchyNode>(entityID2);
    LinkChildToParent(&registry, entityID2, entityID);

    auto children = GetAllChildren(&registry, entityID);
    EXPECT_EQ(children.size(), 1);
    EXPECT_EQ(children[0], entityID2);
    auto children2 = GetAllChildren(&registry, entityID2);
    EXPECT_EQ(children2.size(), 0);
    EXPECT_EQ(registry.GetComponent<HierarchyNode>(entityID2).ParentNode, entityID);
}

TEST(HierarchyTest, ChangeParent)
{
    EntitiesRegistry registry;
    HierarchySystem system;
    registry.RegisterSystem(&system);

    EntityID entityID = registry.CreateNewEntity();
    registry.AddComponent<HierarchyNode>(entityID);
    EntityID entityID2 = registry.CreateNewEntity();
    registry.AddComponent<HierarchyNode>(entityID2);
    EntityID entityID3 = registry.CreateNewEntity();
    registry.AddComponent<HierarchyNode>(entityID3);
    LinkChildToParent(&registry, entityID2, entityID);
    LinkChildToParent(&registry, entityID3, entityID2);

    auto children = GetAllChildren(&registry, entityID);
    EXPECT_EQ(children.size(), 1);
    EXPECT_EQ(children[0], entityID2);
    auto children2 = GetAllChildren(&registry, entityID2);
    EXPECT_EQ(children2.size(), 1);
    EXPECT_EQ(children2[0], entityID3);
    EXPECT_EQ(registry.GetComponent<HierarchyNode>(entityID2).ParentNode, entityID);
    auto children3 = GetAllChildren(&registry, entityID3);
    EXPECT_EQ(children3.size(), 0);
    EXPECT_EQ(registry.GetComponent<HierarchyNode>(entityID3).ParentNode, entityID2);

    LinkChildToParent(&registry, entityID3, entityID);
    children = GetAllChildren(&registry, entityID);
    EXPECT_EQ(children.size(), 2);
    EXPECT_TRUE(std::find(children.begin(), children.end(), entityID2) != children.end());
    EXPECT_TRUE(std::find(children.begin(), children.end(), entityID3) != children.end());
    children2 = GetAllChildren(&registry, entityID2);
    EXPECT_EQ(children2.size(), 0);
    EXPECT_EQ(registry.GetComponent<HierarchyNode>(entityID2).ParentNode, entityID);
    children3 = GetAllChildren(&registry, entityID3);
    EXPECT_EQ(children3.size(), 0);
    EXPECT_EQ(registry.GetComponent<HierarchyNode>(entityID3).ParentNode, entityID);
}

TEST(HierarchyTest, Unlink)
{
    EntitiesRegistry registry;
    HierarchySystem system;
    registry.RegisterSystem(&system);

    EntityID entityID = registry.CreateNewEntity();
    registry.AddComponent<HierarchyNode>(entityID);
    EntityID entityID2 = registry.CreateNewEntity();
    registry.AddComponent<HierarchyNode>(entityID2);
    LinkChildToParent(&registry, entityID2, entityID);
    LinkChildToParent(&registry, entityID2, NULL_ENTITY);

    auto children = GetAllChildren(&registry, entityID);
    EXPECT_EQ(children.size(), 0);
    auto children2 = GetAllChildren(&registry, entityID2);
    EXPECT_EQ(children2.size(), 0);
    EXPECT_EQ(registry.GetComponent<HierarchyNode>(entityID2).ParentNode, NULL_ENTITY);
}

TEST(HierarchyTest, SimpleCicle)
{
    EntitiesRegistry registry;
    HierarchySystem system;
    registry.RegisterSystem(&system);

    EntityID entityID = registry.CreateNewEntity();
    registry.AddComponent<HierarchyNode>(entityID);
    EntityID entityID2 = registry.CreateNewEntity();
    registry.AddComponent<HierarchyNode>(entityID2);
    EntityID entityID3 = registry.CreateNewEntity();
    registry.AddComponent<HierarchyNode>(entityID3);
    LinkChildToParent(&registry, entityID2, entityID);
    LinkChildToParent(&registry, entityID3, entityID2);
    LinkChildToParent(&registry, entityID, entityID3);

    LinkChildToParent(&registry, entityID3, entityID);
    auto children = GetAllChildren(&registry, entityID);
    EXPECT_EQ(children.size(), 2);
    EXPECT_TRUE(std::find(children.begin(), children.end(), entityID2) != children.end());
    EXPECT_TRUE(std::find(children.begin(), children.end(), entityID3) != children.end());
    auto children2 = GetAllChildren(&registry, entityID2);
    EXPECT_EQ(children2.size(), 0);
    EXPECT_EQ(registry.GetComponent<HierarchyNode>(entityID2).ParentNode, entityID);
    auto children3 = GetAllChildren(&registry, entityID3);
    EXPECT_EQ(children3.size(), 0);
    EXPECT_EQ(registry.GetComponent<HierarchyNode>(entityID3).ParentNode, entityID);
}

TEST(HierarchyTest, ComplexCicleAndRebase)
{
    EntitiesRegistry registry;
    HierarchySystem system;
    registry.RegisterSystem(&system);

    EntityID entities[14];
    for (int i = 0; i < 14; ++i)
    {
        entities[i] = registry.CreateNewEntity();
        registry.AddComponent<HierarchyNode>(entities[i]);
    }
    LinkChildToParent(&registry, entities[1], entities[0]);
    LinkChildToParent(&registry, entities[2], entities[0]);
    LinkChildToParent(&registry, entities[3], entities[1]);
    LinkChildToParent(&registry, entities[4], entities[1]);
    LinkChildToParent(&registry, entities[5], entities[2]);
    LinkChildToParent(&registry, entities[6], entities[3]);
    LinkChildToParent(&registry, entities[7], entities[3]);
    LinkChildToParent(&registry, entities[8], entities[5]);
    LinkChildToParent(&registry, entities[9], entities[6]);
    LinkChildToParent(&registry, entities[10], entities[6]);
    LinkChildToParent(&registry, entities[11], entities[6]);
    LinkChildToParent(&registry, entities[12], entities[10]);
    LinkChildToParent(&registry, entities[13], entities[10]);

    EXPECT_EQ(GetAllChildren(&registry, entities[0]).size(), 2);
    EXPECT_EQ(GetAllChildren(&registry, entities[6]).size(), 3);
    EXPECT_EQ(GetAllChildren(&registry, entities[7]).size(), 0);

    LinkChildToParent(&registry, entities[3], entities[13]);
    EXPECT_EQ(GetAllChildren(&registry, entities[0]).size(), 2);
    EXPECT_EQ(GetAllChildren(&registry, entities[6]).size(), 3);
    EXPECT_EQ(GetAllChildren(&registry, entities[7]).size(), 0);
    EXPECT_EQ(GetAllChildren(&registry, entities[13]).size(), 0);
    EXPECT_EQ(registry.GetComponent<HierarchyNode>(entities[3]).ParentNode, entities[1]);

    LinkChildToParent(&registry, entities[0], entities[6]);
    EXPECT_EQ(GetAllChildren(&registry, entities[0]).size(), 2);
    EXPECT_EQ(GetAllChildren(&registry, entities[6]).size(), 3);
    EXPECT_EQ(registry.GetComponent<HierarchyNode>(entities[0]).ParentNode, NULL_ENTITY);

    LinkChildToParent(&registry, entities[6], entities[2]);
    EXPECT_EQ(GetAllChildren(&registry, entities[0]).size(), 2);
    EXPECT_EQ(GetAllChildren(&registry, entities[6]).size(), 3);
    EXPECT_EQ(GetAllChildren(&registry, entities[2]).size(), 2);
    EXPECT_EQ(GetAllChildren(&registry, entities[10]).size(), 2);
    EXPECT_EQ(registry.GetComponent<HierarchyNode>(entities[6]).ParentNode, entities[2]);
    EXPECT_EQ(registry.GetComponent<HierarchyNode>(entities[10]).ParentNode, entities[6]);
}

TEST(HierarchyTest, Remove)
{
    EntitiesRegistry registry;
    HierarchySystem system;
    registry.RegisterSystem(&system);

    EntityID entities[14];
    for (int i = 0; i < 14; ++i)
    {
        entities[i] = registry.CreateNewEntity();
        registry.AddComponent<HierarchyNode>(entities[i]);
    }
    LinkChildToParent(&registry, entities[1], entities[0]);
    LinkChildToParent(&registry, entities[2], entities[0]);
    LinkChildToParent(&registry, entities[3], entities[1]);
    LinkChildToParent(&registry, entities[4], entities[1]);
    LinkChildToParent(&registry, entities[5], entities[2]);
    LinkChildToParent(&registry, entities[6], entities[3]);
    LinkChildToParent(&registry, entities[7], entities[3]);
    LinkChildToParent(&registry, entities[8], entities[5]);
    LinkChildToParent(&registry, entities[9], entities[6]);
    LinkChildToParent(&registry, entities[10], entities[6]);
    LinkChildToParent(&registry, entities[11], entities[6]);
    LinkChildToParent(&registry, entities[12], entities[10]);
    LinkChildToParent(&registry, entities[13], entities[10]);
    registry.DeleteEntity(entities[3]);
    registry.ClearRemoved();

    EXPECT_EQ(GetAllChildren(&registry, entities[0]).size(), 2);
    EXPECT_EQ(GetAllChildren(&registry, entities[1]).size(), 1);
    EXPECT_TRUE(registry.EntityExists(entities[2]));
    EXPECT_TRUE(registry.EntityExists(entities[4]));
    EXPECT_TRUE(registry.EntityExists(entities[5]));
    EXPECT_TRUE(registry.EntityExists(entities[8]));
    EXPECT_FALSE(registry.EntityExists(entities[3]));
    EXPECT_FALSE(registry.EntityExists(entities[6]));
    EXPECT_FALSE(registry.EntityExists(entities[7]));
    EXPECT_FALSE(registry.EntityExists(entities[9]));
    EXPECT_FALSE(registry.EntityExists(entities[10]));
    EXPECT_FALSE(registry.EntityExists(entities[13]));
}
