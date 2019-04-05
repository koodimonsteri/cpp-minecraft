#include "world/chunk.hpp"

// STL
#include <chrono>
#include <vector>
#include <list>
#include <iostream>
#include <tuple>
#include <array>

// 3rd party
#include "glad/glad.h"
#include "glm/gtx/string_cast.hpp"

// Own
#include "game/event/event_util.hpp"
#include "util/bounding_box.hpp"
#include "util/direction.hpp"
#include "util/timer.hpp"
#include "world/worldmanager/worldgenerator.hpp"

void Chunk::updateMesh() {
    /*Renderable renderable = getRenderable();
    Event<const void*, Renderable> e(EventType::UPDATE_RENDERABLE, this, renderable);
    dispatch(e);*/

    RenderableContainer cr = getChunkRenderable();
    if(hasMesh){
        Event<const void*, RenderableContainer> e(EventType::UPDATE_CHUNK_RENDERABLE, this, cr);
        dispatch(e);
    }
}

const Block& Chunk::getBlock(const glm::ivec3& pos) const {
    const auto mod = [](int a, int b) { return (b + (a%b)) % b; };
    return blocks_[mod(pos.x,DIM)][mod(pos.y,DIM)][mod(pos.z,DIM)];
}

RenderableContainer Chunk::getChunkRenderable()
{
    RenderableContainer tempChunkRenderable;
    Renderable solidR = getSolidRenderable();
    Renderable waterR = getWaterRenderable();

    if(solidR.type != RenderableType::EMPTY) { tempChunkRenderable.renderables.push_back(solidR); }
    if(waterR.type != RenderableType::EMPTY) { tempChunkRenderable.renderables.push_back(waterR); }

    if(tempChunkRenderable.renderables.empty()) {
        hasMesh = false;
    } else {
        hasMesh = true;
        tempChunkRenderable.chunkBB = tempChunkRenderable.renderables.at(0).bb;
    }
    //LOG_TRACE("CREATED NEW CHUNKRENDERABLE!  renderables size:", tempChunkRenderable.renderables.size());

    return tempChunkRenderable;
}

Renderable Chunk::getSolidRenderable()
{
    int nof_faces = 0;
    std::vector<glm::tvec3<char>> solidPositions;
    std::vector<char> solidTexIds;
    std::vector<char> solidFaceData;

    const unsigned max_nof_faces = 6*DIM*DIM*DIM;

    solidPositions.reserve(max_nof_faces);
    solidTexIds.reserve(max_nof_faces);
    solidFaceData.reserve(max_nof_faces);

    forEachBlock([&](int x, int y, int z) {
        glm::ivec3 localPos = glm::ivec3(x,y,z);
        glm::ivec3 globalPos = pos_ + localPos;
        const Block& block = blocks_[x][y][z];
        
        if (block.blockType != AIR && block.blockType != WATER) {
            for (auto &d : DirectionUtil::Directions) {
                if (block.hasFace(d)) {
                    glm::tvec3<char> pos = glm::tvec3<char>(x,y,z);
                    solidPositions.push_back(pos);
                    solidTexIds.push_back(BlockUtil::getTextureId(block.blockType, d));
                    solidFaceData.push_back(block.getFaceData(d));
                    ++nof_faces;
                }
            }
        }
    });

    solidPositions.resize(nof_faces);
    solidTexIds.resize(nof_faces);
    solidFaceData.resize(nof_faces);
    
    Renderable renderable;
    renderable.type = RenderableType::EMPTY;
    if(nof_faces > 0) 
    {
        //Renderable renderable;
        glm::mat4 model(1.0f);
        model[3] = glm::vec4(pos_, 1.0f);
        renderable.model = model;
        renderable.size = nof_faces;
        renderable.bb.min = pos_;
        renderable.bb.max = glm::vec3(pos_.x + 16, pos_.y + 16, pos_.z + 16);
        renderable.type = SOLID;

        GLuint vbos[3];
        glGenVertexArrays(1, &renderable.vao);
        glBindVertexArray(renderable.vao);

        glGenBuffers(3, vbos);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glVertexAttribIPointer(0, 3, GL_BYTE, false, 0);
        glBufferData(GL_ARRAY_BUFFER, nof_faces * sizeof(glm::tvec3<char>), &solidPositions.front(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
        glVertexAttribIPointer(1, 1, GL_BYTE, false, 0);
        glBufferData(GL_ARRAY_BUFFER, nof_faces * sizeof(char), &solidTexIds.front(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
        glVertexAttribIPointer(2, 1, GL_BYTE, false, 0);
        glBufferData(GL_ARRAY_BUFFER, nof_faces * sizeof(char), &solidFaceData.front(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glVertexAttribDivisor(0, 1);
        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);

        //glEnableVertexAttribArray(0);
        //glEnableVertexAttribArray(1);
        //glEnableVertexAttribArray(2);

        glBindVertexArray(0);

    }
    return renderable;
}

Renderable Chunk::getWaterRenderable()
{
    int nof_faces = 0;

    std::vector<glm::tvec3<char>> waterPositions;
    std::vector<char> waterTexIds;
    std::vector<char> waterFaceData;

    const unsigned max_nof_faces = 6*DIM*DIM*DIM;

    waterPositions.reserve(max_nof_faces);
    waterTexIds.reserve(max_nof_faces);
    waterFaceData.reserve(max_nof_faces);

    forEachBlock([&](int x, int y, int z) {
        glm::ivec3 localPos = glm::ivec3(x,y,z);
        glm::ivec3 globalPos = pos_ + localPos;
        const Block& block = blocks_[x][y][z];
        
        if (block.blockType == WATER) {
            for (auto &d : DirectionUtil::Directions) {
                if (block.hasFace(d)) {
                    glm::tvec3<char> pos = glm::tvec3<char>(x,y,z);
                    waterPositions.push_back(pos);
                    waterTexIds.push_back(BlockUtil::getTextureId(block.blockType, d));
                    waterFaceData.push_back(block.getFaceData(d));
                    ++nof_faces;
                }
            }
        }
    });

    waterPositions.resize(nof_faces);
    waterTexIds.resize(nof_faces);
    waterFaceData.resize(nof_faces);
    
    Renderable renderable;
    renderable.type = RenderableType::EMPTY;
    if(nof_faces > 0)
    {
        glm::mat4 model(1.0f);
        model[3] = glm::vec4(pos_, 1.0f);
        renderable.model = model;
        renderable.size = nof_faces;
        renderable.bb.min = pos_;
        renderable.bb.max = glm::vec3(pos_.x + 16, pos_.y + 16, pos_.z + 16);
        renderable.type = UNSOLID;
        
        GLuint vbos[3];
        glGenVertexArrays(1, &renderable.vao);
        glBindVertexArray(renderable.vao);

        glGenBuffers(3, vbos);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glVertexAttribIPointer(0, 3, GL_BYTE, false, 0);
        glBufferData(GL_ARRAY_BUFFER, nof_faces * sizeof(glm::tvec3<char>), &waterPositions.front(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
        glVertexAttribIPointer(1, 1, GL_BYTE, false, 0);
        glBufferData(GL_ARRAY_BUFFER, nof_faces * sizeof(char), &waterTexIds.front(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
        glVertexAttribIPointer(2, 1, GL_BYTE, false, 0);
        glBufferData(GL_ARRAY_BUFFER, nof_faces * sizeof(char), &waterFaceData.front(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glVertexAttribDivisor(0, 1);
        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);

        //glEnableVertexAttribArray(0);
        //glEnableVertexAttribArray(1);
        //glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        hasMesh = true;
    }
    return renderable;
}

// r: ray
// min: min block position
// max: max block position
std::optional<Intersection> Chunk::intersectsRay(Ray& r, glm::ivec3 min, glm::ivec3 max) const {
    if (isEmpty(min, max, blocks_)) {
        return {};
    } else {
        if (max == min) {
            Intersection intersection;
            intersection.block = blocks_[min.x][min.y][min.z];
            intersection.blockPos = glm::ivec3(pos_) + min;
            intersection.distance = r.t;
            return intersection;
        } else {
            std::optional<Intersection> intersection = {};
            float min_d = FLT_MAX;
            BoundingBox bb = {glm::ivec3(pos_) + min, glm::ivec3(pos_) + max + glm::ivec3(1)};
            auto subBBs = bb.partition();
            auto subIndices = partition(min, max);
            for (auto i = 0u; i < 8; ++i) {
                if (subBBs[i].intersectsRay(r)) {
                    auto newIntersection = intersectsRay(r, subIndices[i].first, subIndices[i].second);
                    if (newIntersection && newIntersection.value().distance < min_d) {
                        intersection = newIntersection.value();
                        min_d = newIntersection.value().distance;
                    }
                }
            }
            return intersection;
        }
    }
}

std::array<std::pair<glm::ivec3, glm::ivec3>, 8> partition(glm::ivec3 min, glm::ivec3 max) {
    using glm::ivec3;
    ivec3 mid = (max + min) / 2;

    std::array<std::pair<glm::ivec3, glm::ivec3>, 8> ret{{
        std::pair(ivec3(min.x, min.y, min.z), ivec3(mid.x, mid.y, mid.z)),  // (0,0,0)
        std::pair(ivec3(min.x, min.y, 1 + mid.z), ivec3(mid.x, mid.y, max.z)),  // (0,0,1)
        std::pair(ivec3(min.x, 1 + mid.y, min.z), ivec3(mid.x, max.y, mid.z)),  // (0,1,0)
        std::pair(ivec3(min.x, 1 + mid.y, 1 + mid.z), ivec3(mid.x, max.y, max.z)),  // (0,1,1)
        std::pair(ivec3(1 + mid.x, min.y, min.z), ivec3(max.x, mid.y, mid.z)),  // (1,0,0)
        std::pair(ivec3(1 + mid.x, min.y, 1 + mid.z), ivec3(max.x, mid.y, max.z)),  // (1,0,1)
        std::pair(ivec3(1 + mid.x, 1 + mid.y, min.z), ivec3(max.x, max.y, mid.z)),  // (1,1,0)
        std::pair(ivec3(1 + mid.x, 1 + mid.y, 1 + mid.z), ivec3(max.x, max.y, max.z)),  // (1,1,1)
    }};

    return ret;
}

bool isEmpty(glm::ivec3 min, glm::ivec3 max, const Block (&blocks)[Chunk::DIM][Chunk::DIM][Chunk::DIM]) {
    bool empty = true;
    //Chunk::forEachBlock([&](int x, int y, int z) {
    for (auto x = min.x; x <= max.x && empty; ++x) {
        for (auto y = min.y; y <= max.y && empty; ++y) {
            for (auto z = min.z; z <= max.z && empty; ++z) {
                if (blocks[x][y][z].blockType != AIR) {
                    empty = false;
                }
            }
        }
    }
    return empty;
}

bool Chunk::posInBounds(glm::ivec3 pos) const {
    return
        0 <= pos.x && pos.x < DIM &&
        0 <= pos.y && pos.y < DIM &&
        0 <= pos.z && pos.z < DIM;
}

void Chunk::deleteRenderable()
{
    //Event<const void*> e(EventType::DELETE_RENDERABLE, this);
    //dispatch(e);
    Event<const void*> e(EventType::DELETE_CHUNK_RENDERABLE, this);
    dispatch(e);
}