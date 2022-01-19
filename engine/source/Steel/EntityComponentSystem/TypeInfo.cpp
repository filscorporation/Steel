#include "TypeInfo.h"
#include "Steel/Math/Math.h"

TypeInfo::TypeInfo(const char* typeName)
{
    TypeName = typeName;
    ID = Math::StringHash(typeName);
}
