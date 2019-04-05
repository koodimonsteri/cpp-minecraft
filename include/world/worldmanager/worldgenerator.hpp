#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <optional>

#include "glm/glm.hpp"
#include "FastNoise.h"
#include "world/block_type.hpp"
#include "world/chunk_column.hpp"

class ChunkColumn;

/*
 * WorldGenerator handles world generating and saving/loading.
 * At first on main thread, later on its own thread.
 */

class WorldGenerator {
    public:
        WorldGenerator();
        std::unique_ptr<ChunkColumn> generateChunkColumn(glm::ivec2& cPos);
    private:
        BlockType sample(const glm::ivec3& pos) const;
        void      generateQuads(ChunkColumn& chunkColumn) const;
        void      generateBlockTypes(ChunkColumn& chunkColumn) const;
        void      generateBlockData(ChunkColumn& chunkColumn) const;

        FastNoise heightNoise_;
};