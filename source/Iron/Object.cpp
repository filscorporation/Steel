#include "Object.h"

Object::Object()
{
    this->Transform = AddComponent<Transformation>();
}
