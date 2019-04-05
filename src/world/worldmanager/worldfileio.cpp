#include "world/worldmanager/worldfileio.hpp"

#include <fstream>

void WorldFileIO::initWorldSaving(const std::string& fname)
{
    
}

std::unique_ptr<ChunkColumn> WorldFileIO::loadChunkColumn(glm::ivec2& pos)
{
    return nullptr;
}

void WorldFileIO::saveChunkColumn(std::unique_ptr<ChunkColumn> cc)
{

}