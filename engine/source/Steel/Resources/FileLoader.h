#pragma once

#include <cstring>
#include <cstdint>

class FileLoader
{
protected:
    static std::int32_t ConvertToInt(const char* buffer, std::size_t len);
    static bool IsImageTransparent(const unsigned char* imageData, uint32_t width, uint32_t height);
};
