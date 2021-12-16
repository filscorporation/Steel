#pragma once

#include <vector>
#include <glm/glm.hpp>

struct Mesh
{
public:
    bool Validate() const;
    bool IsEmpty() const;
    void Clear();

    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec4> Colors;
    std::vector<glm::vec2> UV;
    std::vector<int> Indices;
};
