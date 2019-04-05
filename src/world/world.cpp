#include "world/world.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "game/event/event_util.hpp"
#include "glm/gtx/string_cast.hpp"
#include "util/direction.hpp"
#include <iostream>
#include <set>
#include <math.h>
#include "util/log.hpp"
#include "util/timer.hpp"

World::World() : player(*this) {
}

World::World(const std::string filename) : player(*this) {
    worldName = filename;
}

World::~World() {
    // Clear data
    /*if(worldManager)
    {
        worldManager->cleanUp();
        delete worldManager;
    }
    if(skyBox)
    {
        delete skyBox;
    }*/
    LOG_TRACE("DELETED WORLD!");
}

void World::init() {
    Timer timer("WorldInit");

    // Construct new WorldManager and initialize it.
    worldManager = new WorldManager(loaderOutput, toBeSaved, *this);
    worldManager->start();

    ambientLight = glm::vec3(0.8, 0.8, 0.8);

    skyBox = new SkyBox();
    skyBox->init();

    fog.active = 0;
    fog.intensity = 0.005;
    fog.color = glm::vec3(0.4, 0.5, 0.4);

    LOG_INFO("World initialized succesfully!");
}

void World::cleanUp()
{
    LOG_INFO("Cleaning up world");
    if(worldManager)
    {
        worldManager->cleanUp();
        delete worldManager;
    }
    if(skyBox)
    {
        delete skyBox;
    }
}

const Block& World::getBlock(const glm::ivec3& pos) const {
    const glm::ivec2 chunkColumnPos = getChunkColumnPos(pos);
    const ChunkColumn& chunkColumn = *(chunkMap.find(chunkColumnPos))->second;
    return chunkColumn.getBlock(pos);
}

void World::destroyBlock(const glm::ivec3& pos) {
    Block& currentBlock = getBlock(pos);
    Chunk& currentChunk = getChunk(pos);
    currentBlock.blockType = AIR;
    currentBlock.blockData = 0;

    std::set<Chunk*> chunksToBeUpdated;
    chunksToBeUpdated.insert(&currentChunk);
    
    int off[3] = {-1, 0, 1};
    for (auto xOff : off) {
        for (auto yOff : off) {
            for (auto zOff : off) {
                if (!(xOff == 0 && yOff == 0 && zOff == 0)) {
                    glm::ivec3 neighborPos = pos + glm::ivec3(xOff, yOff, zOff);
                    Chunk& neighborChunk = getChunk(neighborPos);
                    chunksToBeUpdated.insert(&neighborChunk);
                    updateBlock(neighborPos);
                }
            }
        }
    }

    for (auto chunk : chunksToBeUpdated) {
        chunk->updateMesh();
    }
}

void World::updateBlock(const glm::ivec3& pos) {
    Block& currentBlock = getBlock(pos);
    if (currentBlock.blockType != AIR) {
        for (auto &d : DirectionUtil::Directions) {
            glm::ivec3 neighborPos = pos + DirectionUtil::directionToOffset(d);
            Block& neighbor = getBlock(neighborPos);
            currentBlock.setRenderedFace(d, neighbor.blockType == AIR);
            if (currentBlock.hasFace(d)) {
                for (auto &c : DirectionUtil::getCornersInDirection(d)) {
                    bool isOcluding = false;
                    for (auto &aoOff : DirectionUtil::getAoOffsets(d,c)) {
                        Block& ocludingBlock = getBlock(pos + glm::ivec3(aoOff));
                        if (ocludingBlock.isRendered()) {
                            isOcluding = true;
                            break;
                        }
                    }
                    currentBlock.setOcludedCorner(d, c, isOcluding);
                }
            }
        }
    }
}

std::map<Direction, Block> World::getNeighborBlocks(const glm::ivec3& pos) const {
    std::map<Direction, Block> neighbors;
    for (auto d : DirectionUtil::Directions) {
        glm::ivec3 neighborPos = pos + DirectionUtil::directionToOffset(d);
        auto neighbor = posInBounds(neighborPos) ? getBlock(neighborPos) : Block{AIR};
        neighbors[d] = neighbor;
    }
    return neighbors;
}

bool World::posInBounds(const glm::ivec3& pos) const {
    glm::ivec2 chunkPos = getChunkColumnPos(pos);
    return chunkMap.find(chunkPos) != chunkMap.end() && 0 <= pos.y && pos.y < (ChunkColumn::HEIGHT*Chunk::DIM);
}

const Chunk& World::getChunk(const glm::ivec3& pos) const {
    glm::ivec2 chunkPos = getChunkColumnPos(pos);
    return chunkMap.find(chunkPos)->second->getChunk(pos);
}

Chunk& World::getChunk(const glm::ivec3& pos) {
    glm::ivec2 chunkPos = getChunkColumnPos(pos);
    return chunkMap.find(chunkPos)->second->getChunk(pos);
}

std::optional<Intersection> World::intersectsRay(Ray& r) const {
    glm::ivec2 centerChunkColPos = getChunkColumnPos(player.getPosition());
    unsigned currentChunkIndex = fmax(0, fmin(ChunkColumn::HEIGHT - 1, (int) player.getPosition().y / Chunk::DIM));
    
    std::optional<Intersection> intersection = {};
    float min_d = FLT_MAX;
    int off[] = {-1,0,1};
    for (auto xOff : off) {
        for (auto yOff : off) {
            for (auto zOff : off) {
                glm::ivec2 neighborChunkColPos = centerChunkColPos + glm::ivec2(xOff, zOff);
                if (chunkColumnExists(neighborChunkColPos)) {
                    ChunkColumn& neighborChunkCol = *(chunkMap.find(neighborChunkColPos)->second);
                    unsigned neighborChunkIndex = currentChunkIndex + yOff;
                    if (0 <= neighborChunkIndex && neighborChunkIndex < ChunkColumn::HEIGHT) {
                        const Chunk& neighborChunk = neighborChunkCol.getChunkAt(neighborChunkIndex);
                        std::optional<Intersection> newIntersection = neighborChunk.intersectsRay(r);
                        if (newIntersection && newIntersection.value().distance < min_d) {
                            min_d = newIntersection.value().distance;
                            intersection = newIntersection;
                        }
                    }   
                }
            }
        }
    }
    return intersection;
}

void World::fetchChunkFromLoader()
{
    if(!loaderOutput.q_empty())
    {
        auto entry = loaderOutput.q_pop();
        
        glm::ivec2 chunkPos = entry.first;
        chunkMap.insert(std::move(entry));
        //LOG_INFO("INSERTED CHUNK INTO CHUNKMAP", entry.first.x, " ", entry.first.y);

        ChunkColumn& chunkColumn = *(chunkMap.find(chunkPos))->second;
        chunkColumn.updateMeshes();
    }
}

void World::clearChunkMap()
{
    glm::ivec2 pCPos = getChunkColumnPos(player.getPosition());
    for(auto it = chunkMap.begin(); it != chunkMap.end();)
    {
        glm::ivec2 chunkColPos = it->first;
        if (!chunkColumnPosInLoadArea(chunkColPos)) {
            (*it).second->deleteRenderables();    // Delete renderables
            //toBeSaved.q_push(std::move(*it));     // Push chunk to loader for saving
            chunkMap.erase(it++);                 // Erase from chunkMap
            //LOG_CRITICAL("------- ERASED CHUNKCOLUMN ------", temp);
        } else {
            ++it;
        }
    }
}

bool World::chunkColumnPosInLoadArea(const glm::ivec2& pos) const {
    glm::ivec2 centerChunkColPos = getChunkColumnPos(player.getPosition());

    return 
        centerChunkColPos.x - loadDistance <= pos.x && pos.x <= centerChunkColPos.x + loadDistance &&
        centerChunkColPos.y - loadDistance <= pos.y && pos.y <= centerChunkColPos.y + loadDistance;
}

std::optional<glm::ivec2> World::getClosestMissingChunkColumnPos() const {
    glm::ivec2 playerChunkColpos = getChunkColumnPos(player.getPosition());
    int X = 2*loadDistance + 1;
    int Y = 2*loadDistance + 1;
    int x,y,dx,dy;
    x = y = dx =0;
    dy = -1;
    int t = std::max(X,Y);
    int maxI = t*t;
    for(int i =0; i < maxI; i++){
        if ((-X/2 <= x) && (x <= X/2) && (-Y/2 <= y) && (y <= Y/2)){
            glm::ivec2 off = glm::ivec2(x, y);
            glm::ivec2 chunkColPos = playerChunkColpos + off;

            bool isGenerated = loaderOutput.q_exists([chunkColPos] (const OutputVal& val) -> bool { return val.first == chunkColPos; });

            if(!chunkColumnExists(chunkColPos) && !isGenerated) {
                return chunkColPos;
            }
        }
        if( (x == y) || ((x < 0) && (x == -y)) || ((x > 0) && (x == 1-y))){
            t = dx;
            dx = -dy;
            dy = t;
        }
        x += dx;
        y += dy;
    }
    return {};
}

bool World::chunkColumnExists(const glm::ivec2& pos) const {
    return chunkMap.find(pos) != chunkMap.end();
}

SkyBox* World::getSkyBox() const
{
    if(skyBox) return skyBox;
    else return nullptr;
}

void World::updateChunks() {

}

void World::render() const {
}

void World::update() {
    //Timer timer("Updating world");
    clearChunkMap();          // Clear chunkMap from distant objects and send them back to loader
    fetchChunkFromLoader();   // Get chunks from loader
    player.update();
}

const ChunkMap& World::getChunkMap() const { return chunkMap; }

const Player& World::getPlayer() const { return player; }

const glm::vec3& World::getAmbientLight() const { return ambientLight; }

const Fog& World::getFog() const { return fog; }

glm::ivec2 World::getChunkColumnPos(const glm::vec3& pos) const
{
    //return glm::ivec2((int)pos.x - (int)pos.x%16, (int)pos.z - (int)pos.z % 16);
    return glm::ivec2(floor(pos.x / (float) Chunk::DIM), floor(pos.z / (float) Chunk::DIM));
}

int getIvecDist(const glm::ivec2& v1, const glm::ivec2& v2)
{
    return (int)sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}
