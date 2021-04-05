#include "TestsSetUp.h"

TEST(ApplicationTests, Initialization)
{
    EXPECT_NE(TestApplication::App->Instance, nullptr);
}
