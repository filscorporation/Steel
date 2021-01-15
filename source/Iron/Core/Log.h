#pragma once

#include <string>

class Log
{
public:
    static void LogInfo(const char* message);
    static void LogInfo(const std::string& message);
    static void LogError(const char* message);
    static void LogError(const std::string& message);
};