#pragma once

#include "../EntityComponentSystem/Component.h"
#include "../EntityComponentSystem/Entity.h"

class NameComponent : public Component
{
    DEFINE_TYPE(NameComponent)

public:
    explicit NameComponent(EntityID ownerEntityID) : Component(ownerEntityID) { }

    const std::string& GetName() const;
    void SetName(const std::string& name);
    const std::string& GetTag() const;
    void SetTag(const std::string& tag);

private:
    std::string _name;
    std::string _tag;
};
