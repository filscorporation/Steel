#include <Steel/Platform/Application/AndroidApplication.h>

void android_main(struct android_app* androidApp)
{
    app_dummy(); // Prevent stripping

    // Create application
    AndroidApplication app = AndroidApplication(androidApp);

    // Run application event loop
    app.RunEventLoop();

    // Terminate app
    app.Terminate();
}
