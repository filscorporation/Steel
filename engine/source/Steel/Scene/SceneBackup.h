#pragma once

#include "Steel/Resources/SceneData.h"

#include <yaml-cpp/yaml.h>

struct SceneBackup
{
public:
    SceneBackup(YAML::Node* node, SceneData* data) : Node(node), Data(data) { }

    YAML::Node* Node;
    SceneData* Data;
};
