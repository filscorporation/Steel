#pragma once

#include <vector>
#include "Object.h"

class Scene
{
public:
    std::vector<Object*> Objects;

    Object* CreateObject();
};