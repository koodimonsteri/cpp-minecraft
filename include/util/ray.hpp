#pragma once

#include "glm/glm.hpp"

struct Ray {
    glm::vec3 d;
    glm::vec3 o;
    float t = FLT_MAX;
};