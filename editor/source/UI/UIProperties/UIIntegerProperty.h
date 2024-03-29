#pragma once

#include "TypeNodeWrapper.h"

#include <Steel/EntityComponentSystem/EntitiesRegistry.h>
#include <Steel/EntityComponentSystem/Entity.h>
#include <Steel/EntityComponentSystem/TypeInfo.h>
#include <Steel/Serialization/AttributeInfo.h>
#include <Steel/UI/UILayer.h>

class UIIntegerProperty
{
public:
    static EntityID Create(EntitiesRegistry* entitiesRegistry, UILayer* layer, TypeNodeWrapper* typeNodeWrapper,
                           const TypeInfo* typeInfo, const AttributeInfo& attributeInfo, float& x, float& y, EntityID parent);
};
