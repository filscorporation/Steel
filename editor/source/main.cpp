#include <Steel.h>

#include "EditorCore/EditorApplication.h"

int main()
{
    // Create editor application
    ApplicationSettings settings = ApplicationSettings();
    EditorApplication app = EditorApplication();
    app.Init(settings);

    // Run application main loop
    app.Run();

    return 0;
}