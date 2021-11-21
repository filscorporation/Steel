#include "TestsSetUp.h"
#include "Steel/Scene/Hierarchy.h"

TEST(HierarchyTest, Creation)
{
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entityID = scene->CreateEntity();

    auto children = GetAllChildren(registry, entityID);
    EXPECT_EQ(children.size(), 0);
}

TEST(HierarchyTest, SimpleAdd)
{
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entityID = scene->CreateEntity();
    EntityID entityID2 = scene->CreateEntity();
    LinkChildToParent(registry, entityID2, entityID);

    auto children = GetAllChildren(registry, entityID);
    EXPECT_EQ(children.size(), 1);
    EXPECT_EQ(children[0], entityID2);
    auto children2 = GetAllChildren(registry, entityID2);
    EXPECT_EQ(children2.size(), 0);
    EXPECT_EQ(registry->GetComponent<HierarchyNode>(entityID2).GetParentNode(), entityID);
}

TEST(HierarchyTest, ChangeParent)
{
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entityID = scene->CreateEntity();
    EntityID entityID2 = scene->CreateEntity();
    EntityID entityID3 = scene->CreateEntity();
    LinkChildToParent(registry, entityID2, entityID);
    LinkChildToParent(registry, entityID3, entityID2);

    auto children = GetAllChildren(registry, entityID);
    EXPECT_EQ(children.size(), 1);
    EXPECT_EQ(children[0], entityID2);
    auto children2 = GetAllChildren(registry, entityID2);
    EXPECT_EQ(children2.size(), 1);
    EXPECT_EQ(children2[0], entityID3);
    EXPECT_EQ(registry->GetComponent<HierarchyNode>(entityID2).GetParentNode(), entityID);
    auto children3 = GetAllChildren(registry, entityID3);
    EXPECT_EQ(children3.size(), 0);
    EXPECT_EQ(registry->GetComponent<HierarchyNode>(entityID3).GetParentNode(), entityID2);

    LinkChildToParent(registry, entityID3, entityID);
    children = GetAllChildren(registry, entityID);
    EXPECT_EQ(children.size(), 2);
    EXPECT_TRUE(std::find(children.begin(), children.end(), entityID2) != children.end());
    EXPECT_TRUE(std::find(children.begin(), children.end(), entityID3) != children.end());
    children2 = GetAllChildren(registry, entityID2);
    EXPECT_EQ(children2.size(), 0);
    EXPECT_EQ(registry->GetComponent<HierarchyNode>(entityID2).GetParentNode(), entityID);
    children3 = GetAllChildren(registry, entityID3);
    EXPECT_EQ(children3.size(), 0);
    EXPECT_EQ(registry->GetComponent<HierarchyNode>(entityID3).GetParentNode(), entityID);
}

TEST(HierarchyTest, Unlink)
{
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entityID = scene->CreateEntity();
    EntityID entityID2 = scene->CreateEntity();
    LinkChildToParent(registry, entityID2, entityID);
    LinkChildToParent(registry, entityID2, NULL_ENTITY);

    auto children = GetAllChildren(registry, entityID);
    EXPECT_EQ(children.size(), 0);
    auto children2 = GetAllChildren(registry, entityID2);
    EXPECT_EQ(children2.size(), 0);
    EXPECT_EQ(registry->GetComponent<HierarchyNode>(entityID2).GetParentNode(), NULL_ENTITY);
}

TEST(HierarchyTest, SimpleCicle)
{
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entityID = scene->CreateEntity();
    EntityID entityID2 = scene->CreateEntity();
    EntityID entityID3 = scene->CreateEntity();
    LinkChildToParent(registry, entityID2, entityID);
    LinkChildToParent(registry, entityID3, entityID2);
    LinkChildToParent(registry, entityID, entityID3);

    LinkChildToParent(registry, entityID3, entityID);
    auto children = GetAllChildren(registry, entityID);
    EXPECT_EQ(children.size(), 2);
    EXPECT_TRUE(std::find(children.begin(), children.end(), entityID2) != children.end());
    EXPECT_TRUE(std::find(children.begin(), children.end(), entityID3) != children.end());
    auto children2 = GetAllChildren(registry, entityID2);
    EXPECT_EQ(children2.size(), 0);
    EXPECT_EQ(registry->GetComponent<HierarchyNode>(entityID2).GetParentNode(), entityID);
    auto children3 = GetAllChildren(registry, entityID3);
    EXPECT_EQ(children3.size(), 0);
    EXPECT_EQ(registry->GetComponent<HierarchyNode>(entityID3).GetParentNode(), entityID);
}

TEST(HierarchyTest, ComplexCicleAndRebase)
{
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entities[14];
    for (int i = 0; i < 14; ++i)
    {
        entities[i] = scene->CreateEntity();
    }
    LinkChildToParent(registry, entities[1], entities[0]);
    LinkChildToParent(registry, entities[2], entities[0]);
    LinkChildToParent(registry, entities[3], entities[1]);
    LinkChildToParent(registry, entities[4], entities[1]);
    LinkChildToParent(registry, entities[5], entities[2]);
    LinkChildToParent(registry, entities[6], entities[3]);
    LinkChildToParent(registry, entities[7], entities[3]);
    LinkChildToParent(registry, entities[8], entities[5]);
    LinkChildToParent(registry, entities[9], entities[6]);
    LinkChildToParent(registry, entities[10], entities[6]);
    LinkChildToParent(registry, entities[11], entities[6]);
    LinkChildToParent(registry, entities[12], entities[10]);
    LinkChildToParent(registry, entities[13], entities[10]);

    EXPECT_EQ(GetAllChildren(registry, entities[0]).size(), 2);
    EXPECT_EQ(GetAllChildren(registry, entities[6]).size(), 3);
    EXPECT_EQ(GetAllChildren(registry, entities[7]).size(), 0);

    LinkChildToParent(registry, entities[3], entities[13]);
    EXPECT_EQ(GetAllChildren(registry, entities[0]).size(), 2);
    EXPECT_EQ(GetAllChildren(registry, entities[6]).size(), 3);
    EXPECT_EQ(GetAllChildren(registry, entities[7]).size(), 0);
    EXPECT_EQ(GetAllChildren(registry, entities[13]).size(), 0);
    EXPECT_EQ(registry->GetComponent<HierarchyNode>(entities[3]).GetParentNode(), entities[1]);

    LinkChildToParent(registry, entities[0], entities[6]);
    EXPECT_EQ(GetAllChildren(registry, entities[0]).size(), 2);
    EXPECT_EQ(GetAllChildren(registry, entities[6]).size(), 3);
    EXPECT_EQ(registry->GetComponent<HierarchyNode>(entities[0]).GetParentNode(), NULL_ENTITY);

    LinkChildToParent(registry, entities[6], entities[2]);
    EXPECT_EQ(GetAllChildren(registry, entities[0]).size(), 2);
    EXPECT_EQ(GetAllChildren(registry, entities[6]).size(), 3);
    EXPECT_EQ(GetAllChildren(registry, entities[2]).size(), 2);
    EXPECT_EQ(GetAllChildren(registry, entities[10]).size(), 2);
    EXPECT_EQ(registry->GetComponent<HierarchyNode>(entities[6]).GetParentNode(), entities[2]);
    EXPECT_EQ(registry->GetComponent<HierarchyNode>(entities[10]).GetParentNode(), entities[6]);
}

TEST(HierarchyTest, Remove)
{
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entities[14];
    for (int i = 0; i < 14; ++i)
    {
        entities[i] = scene->CreateEntity();
    }
    LinkChildToParent(registry, entities[1], entities[0]);
    LinkChildToParent(registry, entities[2], entities[0]);
    LinkChildToParent(registry, entities[3], entities[1]);
    LinkChildToParent(registry, entities[4], entities[1]);
    LinkChildToParent(registry, entities[5], entities[2]);
    LinkChildToParent(registry, entities[6], entities[3]);
    LinkChildToParent(registry, entities[7], entities[3]);
    LinkChildToParent(registry, entities[8], entities[5]);
    LinkChildToParent(registry, entities[9], entities[6]);
    LinkChildToParent(registry, entities[10], entities[6]);
    LinkChildToParent(registry, entities[11], entities[6]);
    LinkChildToParent(registry, entities[12], entities[10]);
    LinkChildToParent(registry, entities[13], entities[10]);
    scene->DestroyEntity(entities[3]);
    scene->CleanDestroyedEntities();

    EXPECT_EQ(GetAllChildren(registry, entities[0]).size(), 2);
    EXPECT_EQ(GetAllChildren(registry, entities[1]).size(), 1);
    EXPECT_TRUE(registry->EntityExists(entities[2]));
    EXPECT_TRUE(registry->EntityExists(entities[4]));
    EXPECT_TRUE(registry->EntityExists(entities[5]));
    EXPECT_TRUE(registry->EntityExists(entities[8]));
    EXPECT_FALSE(registry->EntityExists(entities[3]));
    EXPECT_FALSE(registry->EntityExists(entities[6]));
    EXPECT_FALSE(registry->EntityExists(entities[7]));
    EXPECT_FALSE(registry->EntityExists(entities[9]));
    EXPECT_FALSE(registry->EntityExists(entities[10]));
    EXPECT_FALSE(registry->EntityExists(entities[13]));
}
