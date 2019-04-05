#include "world/worldmanager/worldgenerator.hpp"
#include <iostream>
#include <cmath>
#include "util/timer.hpp"
#include "world/block_type.hpp"
#include "glm/gtx/string_cast.hpp"
#include "util/direction.hpp"

WorldGenerator::WorldGenerator() {
    heightNoise_.SetNoiseType(FastNoise::SimplexFractal);
    heightNoise_.SetFrequency(0.005f);
}

std::unique_ptr<ChunkColumn> WorldGenerator::generateChunkColumn(glm::ivec2& cPos)
{
    glm::ivec3 pos = glm::ivec3(cPos.x, 0, cPos.y);
    std::unique_ptr<ChunkColumn> chunkColumn(new ChunkColumn(pos));
    generateBlockTypes(*chunkColumn);
   // floodWater(*chunkColumn);
    generateQuads(*chunkColumn);
    generateBlockData(*chunkColumn);
    return chunkColumn;
}

BlockType WorldGenerator::sample(const glm::ivec3& pos) const {
    float normalized = (heightNoise_.GetNoise(pos.x, pos.y, pos.z) + 1.0f) * 0.5f;
    unsigned height = (unsigned) floor(128.0 * normalized);
    unsigned waterlevel = 40;
    BlockType bt = AIR;
    bt = (pos.y > height) ? (pos.y < waterlevel) ? WATER : AIR : GRASS;
    if(bt == GRASS && pos.y < waterlevel-1) { bt = DIRT; }
    return bt;
}

void WorldGenerator::generateBlockTypes(ChunkColumn& chunkColumn) const {
    ChunkColumn::forEachBlock([&](int x, int y, int z){
        const glm::ivec3 localPos = glm::ivec3(x,y,z);
        Block& block = chunkColumn.getBlock(localPos);
        const glm::ivec3 globalPos = chunkColumn.getPos() + localPos;
        const BlockType blockType = sample(globalPos);
        block.blockType = blockType;
    });
}

void WorldGenerator::generateQuads(ChunkColumn& chunkColumn) const {
    ChunkColumn::forEachBlock([&](int x, int y, int z){
        const glm::ivec3 localPos = glm::ivec3(x, y, z);
        Block& currentBlock = chunkColumn.getBlock(localPos);
        if (currentBlock.blockType != AIR) {
            for (auto &d : DirectionUtil::Directions) {
                const glm::ivec3 off = DirectionUtil::directionToOffset(d);
                const glm::ivec3 neighborPosLocal = localPos + off;
                const glm::ivec3 neighborPosGlobal = chunkColumn.getPos() + neighborPosLocal;
                const auto neighborBlockType = ChunkColumn::posInBounds(neighborPosLocal) ? chunkColumn.getBlock(neighborPosLocal).blockType : sample(neighborPosGlobal); 
                /*if (neighborBlockType == AIR) {
                    currentBlock.setRenderedFace(d, true);
                } */
                if(currentBlock.blockType == WATER && neighborBlockType == AIR){
                    currentBlock.setRenderedFace(d, true);
                } else if(currentBlock.blockType != WATER && (neighborBlockType == AIR || neighborBlockType == WATER)) {
                    currentBlock.setRenderedFace(d, true);
                }
            }
        }
    });
}

void WorldGenerator::generateBlockData(ChunkColumn& chunkColumn) const {
    ChunkColumn::forEachBlock([&](int x, int y, int z){
        const glm::ivec3 localPos = glm::ivec3(x, y, z);
        Block& currentBlock = chunkColumn.getBlock(localPos);
        if (currentBlock.isRendered()) {
            for (auto &d : DirectionUtil::Directions) {
                if (currentBlock.hasFace(d)) {
                    unsigned char aoIndices = 0;
                    auto corners = DirectionUtil::getCornersInDirection(d);
                    for (auto &c : corners) {
                        auto offsets = DirectionUtil::getAoOffsets(d, c);
                        bool isOcluded = false;
                        for (auto &off : offsets) {
                            const glm::ivec3 neighborPosLocal = localPos + glm::ivec3(off);
                            const glm::ivec3 neighborPosGlobal = chunkColumn.getPos() + neighborPosLocal;
                            const BlockType neighborBlockType = ChunkColumn::posInBounds(neighborPosLocal) ? chunkColumn.getBlock(neighborPosLocal).blockType : sample(neighborPosGlobal);
                            if (neighborBlockType != AIR) {
                                isOcluded = true;
                                break;
                            }
                        }
                        if (isOcluded) currentBlock.setOcludedCorner(d, c, true);
                    }
                }
            }
        }
    });
}