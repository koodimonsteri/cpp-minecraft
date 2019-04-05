#pragma once


// STL
#include <array> 
#include <optional>
#include <vector>

// 3rd party
#include "glm/glm.hpp"

// Own
#include "block.hpp"
#include "util/renderable.hpp"
#include "util/intersection.hpp"
#include "util/ray.hpp"
#include "util/block_container.hpp"
#include "util/frustumculling.hpp"

class Chunk : public BlockContainer {
    public:
        Chunk() {}

        std::optional<Intersection> intersectsRay(Ray& r, glm::ivec3 min = glm::ivec3(0), glm::ivec3 max = glm::ivec3(15)) const;

        void updateMesh();
        void deleteRenderable();   // Deletes renderable associated with this chunk

        // Position access
        const glm::ivec3& getPos() const { return pos_; };
        void setPos(const glm::ivec3& pos) { pos_ = pos; };

        // Block data access
        const Block& getBlock(const glm::ivec3& pos) const;
        Block& getBlock(const glm::ivec3& pos) { return BlockContainer::getBlock(pos); };

        RenderableContainer getChunkRenderable();
        Renderable getSolidRenderable();
        Renderable getWaterRenderable();
        
    public: // STATIC
        const static unsigned char DIM = 16;
        template<typename func>
        static void forEachBlock(func f);
    private:
        bool posInBounds(glm::ivec3 pos) const;
        glm::ivec3 pos_;
        Block blocks_[DIM][DIM][DIM];

        bool hasMesh = false;
};

std::array<std::pair<glm::ivec3, glm::ivec3>, 8> partition(glm::ivec3 min, glm::ivec3 max);
bool isEmpty(glm::ivec3 min, glm::ivec3 max, const Block (&blocks)[Chunk::DIM][Chunk::DIM][Chunk::DIM]);

template <typename func>
void Chunk::forEachBlock(func f) {
    for (auto x = 0u; x < Chunk::DIM; ++x) {
        for (auto y = 0u; y < Chunk::DIM; ++y) {
            for (auto z = 0u; z < Chunk::DIM; ++z) {
                f(x,y,z);
            }
        }
    }
}