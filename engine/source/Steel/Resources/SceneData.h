#pragma once

#include <cstdint>

#include "Steel/Resources/Resource.h"

class SceneData : public Resource
{
public:
    explicit SceneData(const std::string& name);
    ~SceneData() override = default;

    const std::string& GetName();

private:
    std::string _name;
};
