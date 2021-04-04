#include "TestsSetUp.h"

int main()
{
    ::testing::InitGoogleTest();
    AddGlobalTestEnvironment(new TestApplication());

    return RUN_ALL_TESTS();
}
