#include <Iron.h>

#include "EditorCore/EditorApplication.h"

int main()
{
    // Create editor application
    ApplicationSettings settings = ApplicationSettings();
    EditorApplication app = EditorApplication(settings);

    // Run application main loop
    app.Run();

    return 0;
}
