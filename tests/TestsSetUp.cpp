#include "TestsSetUp.h"

Application* TestApplication::App;

void TestApplication::SetUp()
{
    ApplicationSettings settings = ApplicationSettings();
    settings.ScreenWidth = 799;
    settings.ScreenHeight = 597;
    App = new Application(settings);
}

void TestApplication::TearDown()
{
    delete App;
}
