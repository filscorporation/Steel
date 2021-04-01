#include <Iron.h>

int main()
{
    Log::LogInfo("Starting");

    ApplicationSettings settings = ApplicationSettings();
    Application app = Application(settings);

    try
    {
        app.Run();
    }
    catch (const std::exception& ex)
    {
        Log::LogError("Error in application: " + std::string(ex.what()));
    }

    Log::LogInfo("Ending");

    return 0;
}
