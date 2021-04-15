#pragma once

#include <cstring>
#include <cstdint>

class FileLoader
{
protected:
    static std::int32_t ConvertToInt(char* buffer, std::size_t len);
};
