#include "TestsSetUp.h"

TEST(ApplicationTest, Initialization)
{
    EXPECT_NE(TestApplication::App->Instance, nullptr);
}
