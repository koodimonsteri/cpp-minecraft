#include "world/chunk_column.hpp"

ChunkColumn::ChunkColumn(const glm::ivec3& pos) : pos_(pos) {
    for (auto i = 0u; i < HEIGHT; ++i) {
        chunks[i].setPos(pos + glm::ivec3(0, i*Chunk::DIM, 0));
    }
}

void ChunkColumn::deleteRenderables()
{
    for(int i = 0; i < HEIGHT; ++i)
    {
        chunks[i].deleteRenderable();
    }
}

void ChunkColumn::updateMeshes()
{
    for(int i = 0; i < HEIGHT; ++i)
    {
        chunks[i].updateMesh();
    }
}

const Block& ChunkColumn::getBlock(const glm::ivec3& pos) const {
    return chunks[pos.y/Chunk::DIM % (HEIGHT)].getBlock(pos);
}

const glm::ivec3& ChunkColumn::getPos() const {
    return pos_;
}

void ChunkColumn::setPos(const glm::ivec3& pos) {
    pos_ = pos;
}

const Chunk& ChunkColumn::getChunk(const glm::ivec3& pos) const {
    return chunks[(int) pos.y / Chunk::DIM];
}

Chunk& ChunkColumn::getChunk(const glm::ivec3& pos) {
    return chunks[(int) pos.y / Chunk::DIM];
}

const Chunk& ChunkColumn::getChunkAt(const unsigned index) const {
    return chunks[index];
}

bool ChunkColumn::posInBounds(const glm::ivec3& pos) {
    return 
        0 <= pos.x && pos.x < Chunk::DIM &&
        0 <= pos.y && pos.y < HEIGHT*Chunk::DIM &&
        0 <= pos.z && pos.z < Chunk::DIM;
}