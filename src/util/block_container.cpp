#include "util/block_container.hpp"

#include <utility>

Block& BlockContainer::getBlock(const glm::ivec3& pos) {
    return const_cast<Block &>(std::as_const(*this).getBlock(pos));
}