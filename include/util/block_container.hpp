#pragma once

#include "glm/glm.hpp"

class Block;

class BlockContainer {
    public:
        BlockContainer() {}
        virtual const Block& getBlock(const glm::ivec3& pos) const = 0;
        virtual Block& getBlock(const glm::ivec3& pos);
};