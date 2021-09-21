#include <Steel.h>

int main()
{
    // Create application with default settings
    ApplicationSettings settings = ApplicationSettings();
    Application app = Application();
    app.Init(settings);

    // Run application main loop
    app.Run();

    return 0;
}
