#include "TestsSetUp.h"
#include "Steel/Scene/Hierarchy.h"
#include "common/TestComponent2.h"

TEST(ActivationTest, Initialization)
{
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entityID = scene->CreateEntity();

    EXPECT_TRUE(registry->EntityGetState(entityID) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entityID) & EntityStates::IsActiveSelf);
}

TEST(ActivationTest, HierarchyActivationDeactivation)
{
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entityID = scene->CreateEntity();
    EntityID entityID2 = scene->CreateEntity();
    LinkChildToParent(registry, entityID2, entityID);
    registry->EntitySetActive(entityID, false, true);

    EXPECT_FALSE(registry->EntityGetState(entityID) & EntityStates::IsActive);
    EXPECT_FALSE(registry->EntityGetState(entityID) & EntityStates::IsActiveSelf);
    EXPECT_FALSE(registry->EntityGetState(entityID2) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entityID2) & EntityStates::IsActiveSelf);

    registry->EntitySetActive(entityID, true, true);

    EXPECT_TRUE(registry->EntityGetState(entityID) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entityID) & EntityStates::IsActiveSelf);
    EXPECT_TRUE(registry->EntityGetState(entityID2) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entityID2) & EntityStates::IsActiveSelf);

    registry->EntitySetActive(entityID2, false, true);

    EXPECT_TRUE(registry->EntityGetState(entityID) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entityID) & EntityStates::IsActiveSelf);
    EXPECT_FALSE(registry->EntityGetState(entityID2) & EntityStates::IsActive);
    EXPECT_FALSE(registry->EntityGetState(entityID2) & EntityStates::IsActiveSelf);

    registry->EntitySetActive(entityID2, true, true);

    EXPECT_TRUE(registry->EntityGetState(entityID) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entityID) & EntityStates::IsActiveSelf);
    EXPECT_TRUE(registry->EntityGetState(entityID2) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entityID2) & EntityStates::IsActiveSelf);
}

TEST(ActivationTest, InheritActivativeStatus)
{
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entities[5];
    for (int i = 0; i < 5; ++i)
    {
        entities[i] = scene->CreateEntity();
    }
    LinkChildToParent(registry, entities[1], entities[0]);
    LinkChildToParent(registry, entities[3], entities[2]);
    LinkChildToParent(registry, entities[4], entities[2]);
    registry->EntitySetActive(entities[3], false, true);

    LinkChildToParent(registry, entities[0], entities[3]);

    EXPECT_FALSE(registry->EntityGetState(entities[3]) & EntityStates::IsActive);
    EXPECT_FALSE(registry->EntityGetState(entities[3]) & EntityStates::IsActiveSelf);
    EXPECT_FALSE(registry->EntityGetState(entities[0]) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entities[0]) & EntityStates::IsActiveSelf);
    EXPECT_FALSE(registry->EntityGetState(entities[1]) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entities[1]) & EntityStates::IsActiveSelf);

    LinkChildToParent(registry, entities[0], entities[4]);

    EXPECT_TRUE(registry->EntityGetState(entities[0]) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entities[0]) & EntityStates::IsActiveSelf);
    EXPECT_TRUE(registry->EntityGetState(entities[1]) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entities[1]) & EntityStates::IsActiveSelf);

    LinkChildToParent(registry, entities[0], entities[3]);

    EXPECT_FALSE(registry->EntityGetState(entities[0]) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entities[0]) & EntityStates::IsActiveSelf);
    EXPECT_FALSE(registry->EntityGetState(entities[1]) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entities[1]) & EntityStates::IsActiveSelf);

    LinkChildToParent(registry, entities[0], NULL_ENTITY);

    EXPECT_TRUE(registry->EntityGetState(entities[0]) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entities[0]) & EntityStates::IsActiveSelf);
    EXPECT_TRUE(registry->EntityGetState(entities[1]) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entities[1]) & EntityStates::IsActiveSelf);

    registry->EntitySetActive(entities[0], false, true);
    LinkChildToParent(registry, entities[0], entities[3]);

    EXPECT_FALSE(registry->EntityGetState(entities[0]) & EntityStates::IsActive);
    EXPECT_FALSE(registry->EntityGetState(entities[0]) & EntityStates::IsActiveSelf);
    EXPECT_FALSE(registry->EntityGetState(entities[1]) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entities[1]) & EntityStates::IsActiveSelf);

    registry->EntitySetActive(entities[0], true, true);

    EXPECT_TRUE(registry->EntityGetState(entities[0]) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entities[0]) & EntityStates::IsActiveSelf);
    EXPECT_TRUE(registry->EntityGetState(entities[1]) & EntityStates::IsActive);
    EXPECT_TRUE(registry->EntityGetState(entities[1]) & EntityStates::IsActiveSelf);
}

TEST(ActivationTest, InsideIteration)
{
    auto scene = Application::Instance->GetCurrentScene();
    auto registry = scene->GetEntitiesRegistry();

    EntityID entities[5];
    for (int i = 0; i < 5; ++i)
    {
        entities[i] = scene->CreateEntity();
        registry->AddComponent<TestComponent2>(entities[i]).IntegerValue = i;
    }
    auto iterator = registry->GetComponentIterator<TestComponent2>();
    auto accessor = registry->GetComponentAccessor<TestComponent2>();
    for (int i = 0; i < iterator.Size(); ++i)
    {
        EXPECT_TRUE(accessor.Has(entities[i]));
        EXPECT_FALSE(accessor.HasInactive(entities[i]));
    }
    int counter = 0;
    for (int i = 0; i < iterator.Size(); ++i)
    {
        ++counter;
        if (i == 1)
        {
            registry->EntitySetActive(entities[1], false, true);
            registry->EntitySetActive(entities[3], false, true);
            EXPECT_FALSE(registry->EntityGetState(entities[i]) & EntityStates::IsActive);
            EXPECT_FALSE(registry->EntityGetState(entities[i]) & EntityStates::IsActiveSelf);
        }
        else if (i == 3)
        {
            EXPECT_FALSE(iterator[i].IsAlive());
            EXPECT_FALSE(registry->EntityGetState(entities[i]) & EntityStates::IsActive);
            EXPECT_FALSE(registry->EntityGetState(entities[i]) & EntityStates::IsActiveSelf);
            EXPECT_FALSE(accessor.Has(entities[i]));
            EXPECT_TRUE(accessor.HasInactive(entities[i]));
            EXPECT_TRUE(accessor.GetInactive(entities[i]).IsAlive());
            EXPECT_EQ(accessor.GetInactive(entities[i]).IntegerValue, i);
        }
        else
        {
            EXPECT_EQ(iterator[i].IntegerValue, i);
            EXPECT_TRUE(registry->EntityGetState(entities[i]) & EntityStates::IsActive);
            EXPECT_TRUE(registry->EntityGetState(entities[i]) & EntityStates::IsActiveSelf);
            EXPECT_TRUE(accessor.Has(entities[i]));
            EXPECT_FALSE(accessor.HasInactive(entities[i]));
        }
    }
}
