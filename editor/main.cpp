#include <Iron.h>

int main()
{
    // Create application with default settings
    ApplicationSettings settings = ApplicationSettings();
    Application app = Application(settings);

    // Run application main loop
    app.Run();

    return 0;
}
