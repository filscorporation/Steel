#include "Steel/EntityComponentSystem/Entity.h"
#include "Steel/Serialization/Serializable.h"
#include "Steel/EntityComponentSystem/TypeInfo.h"

struct UILayoutElementInfo : public Serializable
{
    DEFINE_TYPE(UILayoutElementInfo)

    UILayoutElementInfo() = default;

    EntityID GetElementID() const;
    void SetElementID(EntityID elementID);
    float GetMinWidth() const;
    void SetMinWidth(float minWidth);
    float GetMinHeight() const;
    void SetMinHeight(float minHeight);
    float GetPreferredWidth() const;
    void SetPreferredWidth(float prefWidth);
    float GetPreferredHeight() const;
    void SetPreferredHeight(float prefHeight);

    EntityID ElementID = NULL_ENTITY;
    float MinWidth = 0.0f;
    float MinHeight = 0.0f;
    float PreferredWidth = 0.0f;
    float PreferredHeight = 0.0f;
};
