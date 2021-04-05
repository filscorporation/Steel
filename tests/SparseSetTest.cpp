#include "TestsSetUp.h"
#include "Iron/EntityComponentSystem/SparseSet.h"

TEST(SparseSetTests, Initialization)
{
    SparseSet set;

    EXPECT_EQ(set.Size(), 0);
}

TEST(SparseSetTests, Add)
{
    SparseSet set;

    set.Add(0);
    EXPECT_EQ(set.Size(), 1);
    set.Add(1);
    EXPECT_EQ(set.Size(), 2);
    EXPECT_TRUE(set.Has(0));
    EXPECT_TRUE(set.Has(1));
    EXPECT_FALSE(set.Has(2));
}

TEST(SparseSetTests, RandomAdd)
{
    SparseSet set;
    int values[] = { 3, 745456, 4, 1000, 6, 856348, 15, 9 };
    for (auto value : values)
    {
        set.Add(value);
    }

    EXPECT_EQ(set.Size(), 8);
    EXPECT_TRUE(set.Has(15));
    EXPECT_TRUE(set.Has(856348));
    EXPECT_FALSE(set.Has(856349));
    EXPECT_FALSE(set.Has(0));
}

TEST(SparseSetTests, Remove)
{
    SparseSet set;

    for (uint32_t i = 0; i < 1000; ++i)
    {
        set.Add(i);
    }

    EXPECT_EQ(set.Size(), 1000);
    set.Remove(456);
    EXPECT_EQ(set.Size(), 999);
    EXPECT_FALSE(set.Has(456));
    set.Add(456);
    EXPECT_EQ(set.Size(), 1000);
    EXPECT_TRUE(set.Has(456));
    for (uint32_t i = 700; i < 800; ++i)
    {
        set.Remove(i);
    }
    EXPECT_EQ(set.Size(), 900);
    EXPECT_TRUE(set.Has(800));
    EXPECT_TRUE(set.Has(699));
    EXPECT_TRUE(set.Has(0));
    EXPECT_FALSE(set.Has(700));
    EXPECT_FALSE(set.Has(783));
}

TEST(SparseSetTests, Iterator)
{
    SparseSet set;
    int values[] = { 3, 745456, 4, 1000, 6, 856348, 15, 9 };
    for (auto value : values)
    {
        set.Add(value);
    }

    for (auto value : set)
    {
        EXPECT_TRUE(set.Has(value));
    }
    set.Add(67);
    set.Add(1034);
    set.Remove(856348);
    for (auto value : set)
    {
        EXPECT_TRUE(set.Has(value));
    }
    EXPECT_FALSE(set.Has(856348));
    EXPECT_TRUE(set.Has(1034));
    EXPECT_TRUE(set.Has(3));
}