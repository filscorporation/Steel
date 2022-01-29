#include "TypeInfo.h"
#include "Steel/Math/Math.h"

TypeInfo::TypeInfo(const char* typeName, bool isComponent) : IsComponent(isComponent)
{
    TypeName = typeName;
    ID = Math::StringHash(typeName);
}
