#pragma once

// STL
#include <array>

// 3rd party
#include "glm/glm.hpp"

// Own
#include "util/ray.hpp"

struct BoundingBox {
    bool intersectsRay(Ray& r) const;
    std::array<BoundingBox, 8> partition() const;
    glm::vec3 min;
    glm::vec3 max;
};