#pragma once

#include <memory>

#include "glm/glm.hpp"
#include "world/chunk.hpp"
#include "world/chunk_column.hpp"

class ChunkColumn;

/*
 * namespace that holds all functions related to world saving/loadings
 */

namespace WorldFileIO
{
    void initWorldSaving(const std::string& fname);

    std::unique_ptr<ChunkColumn> loadChunkColumn(glm::ivec2& pos);

    void    saveChunkColumn(std::unique_ptr<ChunkColumn> cc);
};