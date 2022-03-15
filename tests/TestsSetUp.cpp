#include "TestsSetUp.h"
#include "common/TestComponent1.h"
#include "common/TestComponent2.h"
#include "common/TestType.h"
#include "common/SerializedComponent.h"

Application* TestApplication::App;

void TestApplication::SetUp()
{
    TestComponent1::RegisterType();
    TestComponent2::RegisterType();
    TestType::RegisterType();
    SerializedComponent::RegisterType();

    ApplicationSettings settings = ApplicationSettings();
    App = new Application();
    App->Init(settings);
}

void TestApplication::TearDown()
{
    delete App;
}
