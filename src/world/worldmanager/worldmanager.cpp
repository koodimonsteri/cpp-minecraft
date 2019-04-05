#include "world/worldmanager/worldmanager.hpp"

#include <iostream>
#include <exception>

#include "util/timer.hpp"

WorldManager::WorldManager(SafeQueue<OutputVal>& outq, SafeQueue<OutputVal>& saveq, World& world): outputQueue(outq), toBeSavedQueue(saveq), world_(world) {}

WorldManager::~WorldManager() {}

void WorldManager::start()
{
    loaderRunning = true;
    workingThread = std::thread(&WorldManager::loop, this);
}

void WorldManager::loop()
{
    while(loaderRunning)
    {
        auto chunkColPosNorm = world_.getClosestMissingChunkColumnPos();
        if (chunkColPosNorm) {
            glm::ivec2 chunkColPos = chunkColPosNorm.value() * glm::ivec2(16, 16);
            outputQueue.q_push(std::make_pair(chunkColPosNorm.value(), worldGenerator.generateChunkColumn(chunkColPos)));
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void WorldManager::setLoaderRunning(bool b) { loaderRunning = b; }

void WorldManager::cleanUp()
{
    try {
        loaderRunning = false;
        if(workingThread.joinable())
        {
            workingThread.join();
            //delete workingThread;
        }
    } catch(std::exception& e) {
        std::cout << "Found exception on worldmanager cleanUp" << e.what() << std::endl;
    }
}