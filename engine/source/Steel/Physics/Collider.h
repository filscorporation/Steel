#pragma once

#include "../EntityComponentSystem/Component.h"

class Collider : public Component
{
public:
    explicit Collider(EntityID ownerEntityID) : Component(ownerEntityID) { }
    ~Collider() override = default;

protected:
    void OnSizeChanged();

    virtual bool IsSizeValid() const = 0;
};
