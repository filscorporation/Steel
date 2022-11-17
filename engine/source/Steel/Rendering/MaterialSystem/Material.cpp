#include "Material.h"
#include "Steel/Serialization/AttributesRegistration.h"

void Material::RegisterType()
{
    REGISTER_TYPE(Material);
}

Material::Material()
{
    Type = ResourceTypes::Material;
}
