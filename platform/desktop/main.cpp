#include <Steel.h>

int main()
{
    // Create application with default settings
    ApplicationSettings settings = ApplicationSettings();
    Application app = Application();
    app.Init(settings);

    // Run application main loop
    app.PrepareRun();
    app.RunMainLoop();

    // Terminate app
    app.Terminate();

    return 0;
}