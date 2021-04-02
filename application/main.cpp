#include <Iron.h>

int main()
{
    // Create application with default settings
    ApplicationSettings settings = ApplicationSettings();
    Application app = Application(settings);

    // Enable debug window
    Debug::EnableDebugWindow();

    // Run application main loop
    app.Run();

    return 0;
}
