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
