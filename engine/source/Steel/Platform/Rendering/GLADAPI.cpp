#include "GLADAPI.h"

#include <GLAD/glad.h>

int GLADAPI::Init()
{
    return gladLoadGL();
}
