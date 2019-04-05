#pragma once

#include <thread>
#include <atomic>
#include <vector>

#include "util/world_util.hpp"
#include "util/safequeue.hpp"
#include "world/worldmanager/worldgenerator.hpp"
#include "world/world.hpp"

class World;

/*
 * WorldManager class works on its own thread generating, loading and saving chunks.
 * As long as Logger is not thread-safe, WorldManager should not use it
 */

class WorldManager
{
    public:
        WorldManager(SafeQueue<OutputVal>& outq, SafeQueue<OutputVal>& saveq, World& world);
        ~WorldManager();

        void start();        // Start function to initialize thread
        void loop();         // Loop which generates chunks from inputdata
        void cleanUp();      // Save rest chunks to files and terminate thread
        void setLoaderRunning(bool b);

    private:
        std::atomic<bool>        loaderRunning = false;   // Atomic bool for game to signal loader to shutdown
        std::thread              workingThread;           // Thread that does all loading
        SafeQueue<OutputVal>&    outputQueue;             // Thread-safe queue to push generated/loaded chunks to game
        SafeQueue<OutputVal>&    toBeSavedQueue;          // Thread-safe queue to get chunks back from world for saving
        WorldGenerator           worldGenerator;
        World&                   world_;
};
