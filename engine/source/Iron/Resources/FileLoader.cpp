#include "FileLoader.h"

std::int32_t FileLoader::ConvertToInt(char* buffer, std::size_t len)
{
    std::int32_t a = 0;
    int x = 1;
    auto ptr = reinterpret_cast<char*>(&x);
    if (*ptr)
        std::memcpy(&a, buffer, len);
    else
        for (std::size_t i = 0; i < len; ++i)
            reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
    return a;
}

bool FileLoader::IsImageTransparent(const unsigned char* imageData, uint32_t width, uint32_t height)
{
    for (uint32_t i = 0; i < width; ++i)
    {
        for (uint32_t j = 0; j < height; ++j)
        {
            auto alpha = imageData[(i * width + j) * 4 + 3];
            if (alpha != 0 && alpha != 255)
                return true;
        }
    }

    return false;
}
