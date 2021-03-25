#include <iostream>
#include "Log.h"

void Log::LogInfo(const char* message)
{
#ifndef DISTRIBUTE_BUILD
    std::cout << message << std::endl;
#endif
}

void Log::LogInfo(const std::string& message)
{
#ifndef DISTRIBUTE_BUILD
    std::cout << message << std::endl;
#endif
}

void Log::LogInfo(const std::wstring& message)
{
#ifndef DISTRIBUTE_BUILD
    std::wcout << message << std::endl;
#endif
}

void Log::LogWarning(const char* message)
{
#ifndef DISTRIBUTE_BUILD
    std::cout << "WARNING: " << message << std::endl;
#endif
}

void Log::LogWarning(const std::string& message)
{
#ifndef DISTRIBUTE_BUILD
    std::cout << "WARNING: " << message << std::endl;
#endif
}

void Log::LogWarning(const std::wstring& message)
{
#ifndef DISTRIBUTE_BUILD
    std::wcout << L"WARNING: " << message << std::endl;
#endif
}

void Log::LogError(const char* message)
{
#ifndef DISTRIBUTE_BUILD
    std::cout << "ERROR: " << message << std::endl;
#endif
}

void Log::LogError(const std::string& message)
{
#ifndef DISTRIBUTE_BUILD
    std::cout << "ERROR: " << message << std::endl;
#endif
}

void Log::LogError(const std::wstring& message)
{
#ifndef DISTRIBUTE_BUILD
    std::wcout << L"ERROR: " << message << std::endl;
#endif
}
