#include "TestsSetUp.h"
#include "Iron/EntityComponentSystem/SparseDataSet.h"
#include "common/TestComponent1.h"

TEST(SparseDataSetTest, Initialization)
{
    SparseDataSet<TestComponent1> set;

    EXPECT_EQ(set.Size(), 0);
}

TEST(SparseDataSetTest, Add)
{
    SparseDataSet<TestComponent1> set;

    set.Add(0, 0);
    EXPECT_EQ(set.Size(), 1);
    set.Add(1, 1);
    EXPECT_EQ(set.Size(), 2);
    EXPECT_TRUE(set.Has(0));
    EXPECT_TRUE(set.Has(1));
    EXPECT_FALSE(set.Has(2));
    EXPECT_EQ(set.Add(5, 5).Owner, 5);
}

TEST(SparseDataSetTest, RandomAdd)
{
    SparseDataSet<TestComponent1> set;
    int values[] = { 3, 745456, 4, 1000, 6, 856348, 15, 9 };
    for (auto value : values)
    {
        set.Add(value, value);
    }

    EXPECT_EQ(set.Size(), 8);
    EXPECT_TRUE(set.Has(15));
    EXPECT_TRUE(set.Has(856348));
    EXPECT_FALSE(set.Has(856349));
    EXPECT_FALSE(set.Has(0));
}

TEST(SparseDataSetTest, GetAndModify)
{
    SparseDataSet<TestComponent1> set;

    set.Add(0, 0);
    set.Add(1, 1);
    auto& name = set.Get(0);
    name.Owner = 7;
    EXPECT_EQ(set.Get(0).Owner, 7);
    EXPECT_NE(set.Get(1).Owner, 7);
}

TEST(SparseDataSetTest, Remove)
{
    SparseDataSet<TestComponent1> set;

    for (uint32_t i = 0; i < 1000; ++i)
    {
        set.Add(i, i);
    }

    EXPECT_EQ(set.Size(), 1000);
    set.Remove(456);
    EXPECT_EQ(set.Size(), 1000);
    EXPECT_FALSE(set.Has(456));
    set.Condense();
    EXPECT_EQ(set.Size(), 999);
    EXPECT_FALSE(set.Has(456));
    set.Add(456, 456);
    EXPECT_EQ(set.Size(), 1000);
    EXPECT_TRUE(set.Has(456));
    for (uint32_t i = 700; i < 800; ++i)
    {
        set.Remove(i);
    }
    EXPECT_EQ(set.Size(), 1000);
    EXPECT_TRUE(set.Has(800));
    EXPECT_TRUE(set.Has(699));
    EXPECT_TRUE(set.Has(0));
    EXPECT_FALSE(set.Has(700));
    EXPECT_FALSE(set.Has(783));
    set.Condense();
    EXPECT_EQ(set.Size(), 900);
    EXPECT_TRUE(set.Has(800));
    EXPECT_TRUE(set.Has(699));
    EXPECT_TRUE(set.Has(0));
    EXPECT_FALSE(set.Has(700));
    EXPECT_FALSE(set.Has(783));
    set.Condense();
    EXPECT_EQ(set.Size(), 900);
}

TEST(SparseDataSetTest, Iterator)
{
    SparseDataSet<TestComponent1> set;
    uint32_t values[] = { 3, 745456, 4, 1000, 6, 856348, 15, 9 };
    for (auto value : values)
    {
        set.Add(value, value);
    }

    for (auto& value : set)
    {
        EXPECT_TRUE(set.Has(value.Owner));
    }
    set.Add(67, 67);
    set.Add(1034, 1034);
    set.Remove(856348);
    for (auto& value : set)
    {
        if (value.Owner == NULL_ENTITY)
        {
            EXPECT_FALSE(set.Has(value.Owner));
        }
        else
        {
            EXPECT_TRUE(set.Has(value.Owner));
        }
    }
    EXPECT_FALSE(set.Has(856348));
    EXPECT_TRUE(set.Has(1034));
    EXPECT_TRUE(set.Has(3));
    set.Condense();
    for (auto& value : set)
    {
        if (value.Owner == 4 || value.Owner == 3)
        {
            auto temp = value.Owner;
            set.Remove(value.Owner);
            EXPECT_EQ(value.Owner, NULL_ENTITY);
            EXPECT_FALSE(set.Has(temp));
        }
        else
        {
            EXPECT_TRUE(set.Has(value.Owner));
        }
    }
}

TEST(SparseDataSetTest, Indexing)
{
    SparseDataSet<TestComponent1> set;
    set.Add(0, 0);
    set.Add(1, 1);
    set.Add(2, 2);

    for (int i = 0; i < set.Size(); ++i)
    {
        EXPECT_EQ(set[i].Owner, i);
    }
    set.Remove(1);
    for (int i = 0; i < set.Size(); ++i)
    {
        if (set[i].Owner == NULL_ENTITY)
            EXPECT_EQ(1, i);
        else
            EXPECT_EQ(set[i].Owner, i);
    }
    set.Condense();
    EXPECT_EQ(set[0].Owner, 0);
    EXPECT_EQ(set[1].Owner, 2);
    EXPECT_EQ(set.Size(), 2);
}
