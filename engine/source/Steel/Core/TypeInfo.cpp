#include "TypeInfo.h"
#include "../Math/Math.h"

TypeInfo::TypeInfo(const char* typeName)
{
    TypeName = typeName;
    ID = Math::StringHash(typeName);
}
