#include "AsepriteData.h"
#include "Steel/Serialization/AttributesRegistration.h"

void AsepriteData::RegisterType()
{
    REGISTER_TYPE(AsepriteData);
}

AsepriteData::AsepriteData()
{
    Type = ResourceTypes::AsepriteData;
}
