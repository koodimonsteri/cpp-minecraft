#pragma once

#include "glm/glm.hpp"

struct Fog {
    int       active;     // Is fog enabled
    float     intensity;  // Intensity of the fog
    glm::vec3 color;      // Color of the fog
};