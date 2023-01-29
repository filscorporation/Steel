#include <Steel.h>

#include "EditorCore/EditorApplication.h"

int main()
{
    // Create editor application
    ApplicationSettings settings = ApplicationSettings();
    EditorApplication app = EditorApplication();
    app.Init(settings);

    // Run application main loop
    app.PrepareRun();
    app.RunMainLoop();

    // Terminate application
    app.Terminate();

    return 0;
}
