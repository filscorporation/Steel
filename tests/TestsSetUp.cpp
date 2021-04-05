#include "TestsSetUp.h"

Application* TestApplication::App;

void TestApplication::SetUp()
{
    ApplicationSettings settings = ApplicationSettings();
    App = new Application(settings);
}

void TestApplication::TearDown()
{
    delete App;
}
