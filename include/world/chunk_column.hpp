#pragma once

// 3rd party
#include "glm/glm.hpp"

// Own
#include "util/block_container.hpp"
#include "world/chunk.hpp"

class Chunk;

class ChunkColumn : public BlockContainer {
    public:
        ChunkColumn(const glm::ivec3& pos);

        // Block data access
        const Block& getBlock(const glm::ivec3& pos) const;
        Block& getBlock(const glm::ivec3& pos) { return BlockContainer::getBlock(pos); };

        const Chunk& getChunk(const glm::ivec3& pos) const;
        Chunk& getChunk(const glm::ivec3& pos);
        const Chunk& getChunkAt(const unsigned index) const;

        // Position access
        const glm::ivec3& getPos() const;
        void              setPos(const glm::ivec3& pos);

        void deleteRenderables();   // Deletes all renderables from ChunkColumn
        void updateMeshes();

    public: // STATIC
        const static unsigned char HEIGHT = 8;
        static bool posInBounds(const glm::ivec3& pos);
        template<typename func>
        static void forEachBlock(func f);
    private:
        Chunk chunks[HEIGHT];
        glm::ivec3 pos_;
};

template <typename func>
void ChunkColumn::forEachBlock(func f) {
    for (auto x = 0u; x < Chunk::DIM; ++x) {
        for (auto y = 0u; y < Chunk::DIM*HEIGHT; ++y) {
            for (auto z = 0u; z < Chunk::DIM; ++z) {
                f(x,y,z);
            }
        }
    }
}