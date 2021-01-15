#include "Object.h"

Object::Object()
{
    this->Transform = AddComponent<Transformation>();
}

Object::~Object()
{
    for (auto component : Components())
    {
        free(component);
    }
}
