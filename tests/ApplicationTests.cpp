#include "TestsSetUp.h"

TEST(TestApplication, Initialization)
{
    EXPECT_NE(TestApplication::App->Instance, nullptr);
}

TEST(TestApplication, ScreenWidth)
{
    EXPECT_EQ(Screen::GetWidth(), 799);
}

TEST(TestApplication, ScreenHeight)
{
    EXPECT_EQ(Screen::GetHeight(), 597);
}
