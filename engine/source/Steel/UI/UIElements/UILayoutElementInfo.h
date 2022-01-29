#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/Serialization/Serializable.h"

struct UILayoutElementInfo : public Serializable
{
    DEFINE_TYPE(UILayoutElementInfo)

    UILayoutElementInfo() = default;
    explicit UILayoutElementInfo(EntityID elementID);

    EntityID GetElementID() const;
    void SetElementID(EntityID elementID);

    EntityID ElementID = NULL_ENTITY;
};