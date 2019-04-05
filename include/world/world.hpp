#pragma once

// STL
#include <string>
#include <map>
#include <optional>
#include <unordered_map>
#include <vector>
#include <memory>

// Own
#include "util/block_container.hpp"
#include "util/world_util.hpp"
#include "util/direction.hpp"
#include "world/chunk_column.hpp"
#include "game/player.hpp"
#include "world/worldmanager/worldgenerator.hpp"
#include "world/worldmanager/worldmanager.hpp"
#include "world/skybox.hpp"
#include "util/safequeue.hpp"
#include "util/ivec2_map.hpp"
#include "world/fog.hpp"

//class WorldGenerator;
class WorldManager;

using ChunkMap = ivec2_map<std::unique_ptr<ChunkColumn>>;

class World : public BlockContainer {
    public:
        World();
        World(const std::string filename);
        ~World();
        void init();
        void cleanUp();
        void render() const;
        void update();
        void updateChunks();

        std::optional<Intersection> intersectsRay(Ray& r) const;    

        //const Chunk& getChunk(const glm::ivec3& pos) const;
        glm::ivec2 getChunkColumnPos(const glm::vec3& pos) const;

         // Block data access
        const Block& getBlock(const glm::ivec3& pos) const;
        Block& getBlock(const glm::ivec3& pos) { return BlockContainer::getBlock(pos); };

        void destroyBlock(const glm::ivec3& pos);
        void updateBlock(const glm::ivec3& pos);

        const Chunk& getChunk(const glm::ivec3& pos) const;
        Chunk& getChunk(const glm::ivec3& pos);

        std::map<Direction, Block> getNeighborBlocks(const glm::ivec3& pos) const;
        bool posInBounds(const glm::ivec3& pos) const;

        std::optional<glm::ivec2> getClosestMissingChunkColumnPos() const;
        bool chunkColumnExists(const glm::ivec2& pos) const;
        bool chunkColumnPosInLoadArea(const glm::ivec2& pos) const;

        void             updateChunkMap();
        void             fetchChunkFromLoader();
        void             clearChunkMap();
        void             addChunkToMap(std::unique_ptr<ChunkColumn> cptr);
        const ChunkMap&  getChunkMap() const;
        const Player&    getPlayer() const;
        const glm::vec3& getAmbientLight() const;
        const Fog&       getFog() const;
        SkyBox*          getSkyBox() const;
        
    private:
        Player         player;
        WorldGenerator worldGenerator;
        //int            renderDistance = 5;
        int            loadDistance   = 11;
        ChunkMap       chunkMap;
        WorldManager*  worldManager = nullptr;
        SkyBox*        skyBox = nullptr;
        glm::vec3      ambientLight;
        Fog            fog;

        SafeQueue<OutputVal> loaderOutput;  // Output queue to receive chunks from loader
        SafeQueue<OutputVal> toBeSaved;     // Queue to push chunks back to loader for saving

        std::string worldName;
};

int getIvecDist(const glm::ivec2& v1,const glm::ivec2& v2);
