#include "TestsSetUp.h"

Application* TestApplication::App;

void TestApplication::SetUp()
{
    ApplicationSettings settings = ApplicationSettings();
    App = new Application();
    App->Init(settings);
}

void TestApplication::TearDown()
{
    delete App;
}
