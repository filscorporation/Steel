#include "TestsSetUp.h"
#include "common/TestComponent1.h"
#include "common/TestComponent2.h"
#include "common/TestComponent1System.h"

TEST(EntitiesRegistryTest, Entities)
{
    EntitiesRegistry registry;

    EXPECT_FALSE(registry.EntityExists(0));
    EntityID entityID = registry.CreateNewEntity();
    EXPECT_TRUE(registry.EntityExists(entityID));
    registry.DeleteEntity(entityID);
    EXPECT_FALSE(registry.EntityExists(entityID));

    EntityID entityID1 = registry.CreateNewEntity();
    EntityID entityID2 = registry.CreateNewEntity();
    EXPECT_FALSE(registry.EntityExists(entityID));
    EXPECT_NE(entityID1, entityID2);
    entityID = registry.CreateNewEntity();
    EXPECT_TRUE(registry.EntityExists(entityID));
    registry.CleanAllEntities();
    EXPECT_FALSE(registry.EntityExists(entityID));
}

TEST(EntitiesRegistryTest, Versions)
{
    EntitiesRegistry registry;

    registry.CreateNewEntity();
    EntityID entityID = registry.CreateNewEntity();
    registry.CreateNewEntity();
    EXPECT_EQ(entityID, registry.EntityActual(registry.EntityIDGetID(entityID)));
    EXPECT_EQ(entityID, EntitiesRegistry::EntityIDCombine(
            registry.EntityIDGetID(entityID), registry.EntityIDGetVersion(entityID)));

    auto version = EntitiesRegistry::EntityIDGetVersion(entityID);
    auto id = EntitiesRegistry::EntityIDGetID(entityID);
    registry.DeleteEntity(entityID);
    entityID = registry.CreateNewEntity();
    EXPECT_NE(version, registry.EntityIDGetVersion(entityID));
    EXPECT_EQ(id, registry.EntityIDGetID(entityID));
}

TEST(EntitiesRegistryTest, VersionOverflow)
{
    EntitiesRegistry registry;

    EntityID entityID = registry.CreateNewEntity();
    for (uint32_t i = 0; i < (uint32_t)0xFFF; ++i)
    {
        registry.DeleteEntity(registry.CreateNewEntity());
    }
    EXPECT_TRUE(registry.EntityExists(entityID));
    EntityID newEntityID = registry.CreateNewEntity();
    EXPECT_TRUE(registry.EntityExists(newEntityID));
    EXPECT_TRUE(registry.EntityExists(entityID));
    EXPECT_NE(entityID, newEntityID);
    registry.CleanAllEntities();
    EXPECT_FALSE(registry.EntityExists(entityID));
    EXPECT_FALSE(registry.EntityExists(newEntityID));
}

TEST(EntitiesRegistryTest, AddGetComponents)
{
    EntitiesRegistry registry;

    EntityID entityID = registry.CreateNewEntity();
    EXPECT_FALSE(registry.HasComponent<TestComponent1>(entityID));
    EXPECT_FALSE(registry.HasComponent<TestComponent2>(entityID));
    registry.AddComponent<TestComponent1>(entityID);
    EXPECT_TRUE(registry.HasComponent<TestComponent1>(entityID));
    EXPECT_FALSE(registry.HasComponent<TestComponent2>(entityID));
    registry.GetComponent<TestComponent1>(entityID).IntegerValue = 7;
    EXPECT_EQ(registry.GetComponent<TestComponent1>(entityID).IntegerValue, 7);
    EntityID entityID2 = registry.CreateNewEntity();
    registry.AddComponent<TestComponent1>(entityID2).IntegerValue = -3;
    EXPECT_EQ(registry.GetComponent<TestComponent1>(entityID2).IntegerValue, -3);
    EXPECT_EQ(registry.GetComponent<TestComponent1>(entityID).IntegerValue, 7);
    EXPECT_EQ(registry.AddComponent<TestComponent1>(entityID).IntegerValue, 7);
    registry.AddComponent<TestComponent2>(entityID2).IntegerValue = -11;
    EXPECT_FALSE(registry.HasComponent<TestComponent2>(entityID));
    EXPECT_TRUE(registry.HasComponent<TestComponent2>(entityID2));
    EXPECT_EQ(registry.GetComponent<TestComponent1>(entityID2).IntegerValue, -3);
    EXPECT_EQ(registry.GetComponent<TestComponent2>(entityID2).IntegerValue, -11);
}

TEST(EntitiesRegistryTest, RemoveComponents)
{
    EntitiesRegistry registry;

    EntityID entityID = registry.CreateNewEntity();
    registry.AddComponent<TestComponent1>(entityID);
    registry.AddComponent<TestComponent2>(entityID);
    registry.RemoveComponent<TestComponent1>(entityID);
    EXPECT_FALSE(registry.HasComponent<TestComponent1>(entityID));
    EXPECT_TRUE(registry.HasComponent<TestComponent2>(entityID));
    registry.AddComponent<TestComponent1>(entityID);
    EXPECT_TRUE(registry.HasComponent<TestComponent1>(entityID));

    registry.GetComponent<TestComponent1>(entityID).IntegerValue = 2;
    registry.RemoveComponent<TestComponent1>(entityID);
    registry.AddComponent<TestComponent1>(entityID);
    EXPECT_NE(registry.GetComponent<TestComponent1>(entityID).IntegerValue, 2);

    registry.DeleteEntity(entityID);
    entityID = registry.CreateNewEntity();
    EXPECT_FALSE(registry.HasComponent<TestComponent1>(entityID));
    registry.AddComponent<TestComponent1>(entityID);
    EXPECT_TRUE(registry.HasComponent<TestComponent1>(entityID));
    EXPECT_FALSE(registry.HasComponent<TestComponent2>(entityID));
    registry.RemoveComponent<TestComponent1>(entityID);
    EXPECT_FALSE(registry.HasComponent<TestComponent1>(entityID));
}

TEST(EntitiesRegistryTest, ClearRemoved)
{
    EntitiesRegistry registry;

    EntityID entityID = registry.CreateNewEntity();
    registry.AddComponent<TestComponent1>(entityID);
    registry.AddComponent<TestComponent2>(entityID);
    registry.RemoveComponent<TestComponent1>(entityID);
    EXPECT_FALSE(registry.HasComponent<TestComponent1>(entityID));
    registry.ClearRemoved();
    EXPECT_FALSE(registry.HasComponent<TestComponent1>(entityID));
    registry.AddComponent<TestComponent1>(entityID);
    EXPECT_TRUE(registry.HasComponent<TestComponent1>(entityID));
    registry.ClearRemoved();
    EXPECT_TRUE(registry.HasComponent<TestComponent1>(entityID));
    registry.RemoveComponent<TestComponent1>(entityID);
    registry.AddComponent<TestComponent1>(entityID);
    registry.ClearRemoved();
    EXPECT_TRUE(registry.HasComponent<TestComponent1>(entityID));
    registry.RemoveComponent<TestComponent1>(entityID);
    EXPECT_FALSE(registry.HasComponent<TestComponent1>(entityID));
    registry.ClearRemoved<TestComponent1>();
    EXPECT_FALSE(registry.HasComponent<TestComponent1>(entityID));
}

TEST(EntitiesRegistryTest, ComponentSystems)
{
    EntitiesRegistry registry;

    TestComponent1System system;
    registry.RegisterSystem<TestComponent1>(&system);
    registry.RemoveSystem<TestComponent1>();
    EntityID entityID = registry.CreateNewEntity();
    registry.AddComponent<TestComponent1>(entityID);
    EXPECT_EQ(registry.GetComponent<TestComponent1>(entityID).IntegerValue, 0);

    registry.RegisterSystem<TestComponent1>(&system);
    EntityID entityID1 = registry.CreateNewEntity();
    registry.AddComponent<TestComponent1>(entityID1);
    EXPECT_EQ(registry.GetComponent<TestComponent1>(entityID1).IntegerValue, 7);
    registry.DeleteEntity(entityID);
    registry.RemoveComponent<TestComponent1>(entityID1);
    EXPECT_EQ(system.CatchAdded, 1);
    EXPECT_EQ(system.CatchRemoved, 2);

    entityID = registry.CreateNewEntity();
    registry.AddComponent<TestComponent1>(entityID);
    entityID1 = registry.CreateNewEntity();
    registry.AddComponent<TestComponent1>(entityID1).IntegerValue = 4;
    registry.EntitySetActive(entityID, false, true);
    EXPECT_EQ(registry.GetComponent<TestComponent1>(entityID).IntegerValue, 9);
    EXPECT_EQ(registry.GetComponent<TestComponent1>(entityID1).IntegerValue, 4);
    registry.EntitySetActive(entityID, true, true);
    EXPECT_EQ(registry.GetComponent<TestComponent1>(entityID).IntegerValue, 11);
    EXPECT_EQ(registry.GetComponent<TestComponent1>(entityID1).IntegerValue, 4);
    EXPECT_EQ(system.CatchEnabled, 1);
    EXPECT_EQ(system.CatchDisabled, 1);
}

TEST(EntitiesRegistryTest, Iterator)
{
    EntitiesRegistry registry;

    for (int i = 0; i < 30; ++i)
    {
        EntityID entityID = registry.CreateNewEntity();
        registry.AddComponent<TestComponent1>(entityID).IntegerValue = 1;
    }
    for (int i = 0; i < 20; ++i)
    {
        EntityID entityID = registry.CreateNewEntity();
        registry.AddComponent<TestComponent2>(entityID).IntegerValue = 2;
    }
    for (int i = 0; i < 10; ++i)
    {
        EntityID entityID = registry.CreateNewEntity();
        registry.AddComponent<TestComponent1>(entityID).IntegerValue = 3;
        registry.AddComponent<TestComponent2>(entityID).IntegerValue = 3;
    }
    auto iterator1 = registry.GetComponentIterator<TestComponent1>();
    auto iterator2 = registry.GetComponentIterator<TestComponent2>();

    EXPECT_EQ(iterator1.Size(), 30 + 10);
    EXPECT_EQ(iterator2.Size(), 20 + 10);

    int counter1 = 0, counter2 = 0, counter3 = 0;
    for (auto& component : iterator1)
    {
        if (component.IntegerValue == 1) ++counter1;
        if (component.IntegerValue == 2) ++counter2;
        if (component.IntegerValue == 3) ++counter3;

        component.IntegerValue = -1;
    }
    EXPECT_EQ(counter1, 30);
    EXPECT_EQ(counter2, 0);
    EXPECT_EQ(counter3, 10);
    for (auto& component : iterator1)
    {
        EXPECT_EQ(component.IntegerValue, -1);
    }

    int counter = 0;
    registry.RemoveComponent<TestComponent2>(iterator2[0].Owner);
    EntityID entityID = registry.CreateNewEntity();
    registry.AddComponent<TestComponent2>(entityID).IntegerValue = 6;
    for (int i = 0; i < iterator2.Size(); ++i)
    {
        if (iterator2[i].IsAlive())
        {
            iterator2[i].IntegerValue = 5;
            counter ++;
        }
    }
    EXPECT_EQ(counter, 30);
    registry.ClearRemoved<TestComponent2>();
    for (auto& component : iterator2)
    {
        EXPECT_EQ(component.IntegerValue, 5);
    }
}

TEST(EntitiesRegistryTest, Accessor)
{
    EntitiesRegistry registry;

    EntityID entityID = registry.CreateNewEntity();
    registry.AddComponent<TestComponent1>(entityID).IntegerValue = 8;
    registry.AddComponent<TestComponent1>(registry.CreateNewEntity());
    registry.AddComponent<TestComponent1>(registry.CreateNewEntity());
    registry.AddComponent<TestComponent1>(registry.CreateNewEntity());
    registry.AddComponent<TestComponent1>(registry.CreateNewEntity());
    EntityID entityID1 = registry.CreateNewEntity();
    registry.AddComponent<TestComponent2>(entityID1);
    registry.AddComponent<TestComponent2>(registry.CreateNewEntity());
    registry.AddComponent<TestComponent2>(registry.CreateNewEntity());
    registry.AddComponent<TestComponent2>(registry.CreateNewEntity());
    EntityID entityID2 = registry.CreateNewEntity();
    registry.AddComponent<TestComponent1>(entityID2);
    registry.AddComponent<TestComponent2>(entityID2);
    EntityID entityID3 = registry.CreateNewEntity();
    registry.AddComponent<TestComponent1>(entityID3);
    registry.AddComponent<TestComponent2>(entityID3);
    registry.EntitySetActive(entityID3, false, true);
    auto accessor1 = registry.GetComponentAccessor<TestComponent1>();
    auto accessor2 = registry.GetComponentAccessor<TestComponent2>();

    EXPECT_TRUE(accessor1.Has(entityID));
    EXPECT_EQ(accessor1.Get(entityID).IntegerValue, 8);
    EXPECT_FALSE(accessor2.Has(entityID));
    EXPECT_FALSE(accessor1.Has(entityID1));
    EXPECT_TRUE(accessor2.Has(entityID1));
    accessor2.Get(entityID1).IntegerValue = -2;
    EXPECT_EQ(accessor2.Get(entityID1).IntegerValue, -2);
    EXPECT_TRUE(accessor1.Has(entityID2));
    EXPECT_TRUE(accessor2.Has(entityID2));
    EXPECT_FALSE(accessor1.HasInactive(entityID2));
    EXPECT_FALSE(accessor2.HasInactive(entityID2));
    EXPECT_FALSE(accessor1.Has(entityID3));
    EXPECT_FALSE(accessor2.Has(entityID3));
    EXPECT_TRUE(accessor1.HasInactive(entityID3));
    EXPECT_TRUE(accessor2.HasInactive(entityID3));
}

TEST(EntitiesRegistryTest, Sorting)
{
    EntitiesRegistry registry;

    int values[] = { 73, 8, 94, 1, 57, 60, 19, 49, 10, 35, 90, 8, 8, 7, 29, 43, 9, 73, 17, 19 };
    for (int i = 0; i < 20; i ++)
    {
        EntityID entityID = registry.CreateNewEntity();
        registry.AddComponent<TestComponent1>(entityID).IntegerValue = values[i];
        registry.AddComponent<TestComponent2>(entityID).IntegerValue = i;
    }
    EntityID entityID = registry.CreateNewEntity();
    registry.AddComponent<TestComponent1>(entityID).IntegerValue = 50;
    struct
    {
        bool operator()(const TestComponent1& a, const TestComponent1& b) const { return a.IntegerValue < b.IntegerValue; }
    } TestComparer;
    registry.SortComponents<TestComponent1>(TestComparer);

    auto iterator1 = registry.GetComponentIterator<TestComponent1>();
    for (int i = 1; i < 21; i ++)
    {
        EXPECT_LE(iterator1[i - 1].IntegerValue, iterator1[i].IntegerValue);
    }
    registry.ApplyOrder<TestComponent1, TestComponent2>();
    auto iterator2 = registry.GetComponentIterator<TestComponent1>();
    iterator2[0].IntegerValue = 3;
    iterator2[19].IntegerValue = 2;
}
