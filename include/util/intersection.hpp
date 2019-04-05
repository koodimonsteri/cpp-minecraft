#pragma once

// 3rd party
#include "glm/glm.hpp"

// Own
#include "world/block.hpp"
#include "world/chunk.hpp"

class Chunk;

struct Intersection {
    Block block;
    glm::ivec3 blockPos;
    float distance = FLT_MAX;
};