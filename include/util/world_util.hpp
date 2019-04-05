#pragma once

#include <tuple>
#include <memory>

#include "glm/glm.hpp"

class ChunkColumn;

// Data types for input and ouput queues
typedef std::pair<glm::ivec2, float>                        InputVal;
typedef std::pair<glm::ivec2, std::unique_ptr<ChunkColumn>> OutputVal;