#include "game/game.hpp"

// STL
#include <chrono>
#include <iostream>

// 3rd party
#include "glm/glm.hpp"

// Own
#include "game/event/event_util.hpp"
#include "game/config.hpp"
#include "util/log.hpp"
#include "util/timer.hpp"
#include "util/math_constants.hpp"

Game::Game(): window_(gameConfig) {}

void Game::init() {
    // Event callbacks
    using namespace std::placeholders;
    EventHandler<bool*>::appendAction(EventType::E_KEY, std::bind(&Game::keyCallback, this, _1));
    EventHandler<float, float>::appendAction(EventType::E_MOUSE, std::bind(&Game::mouseCallback, this, _1, _2));
    EventHandler<>::bindAction(EventType::APPLICATION_CLOSE, std::bind(&Game::close, this));

    window_.init();
    renderer_.init();
    renderer_.setRenderDistance(gameConfig.renderDistance * Chunk::DIM);
    loadWorld(std::string());
    world_->init();
}

void Game::keyCallback(bool* keysPressed) {
    static int i = 0;
    if (keysPressed[KEY_ESCAPE]) {
        //Event e(EventType::APPLICATION_CLOSE);
        //dispatch(e);
        window_.toggleCursor();
    } else if(keysPressed[KEY_F]){
        window_.toggleFullScreen();
    }
}

void Game::mouseCallback(float dx, float dy) {

}

void Game::start() {
    Timer m_timer("Total game time");
    running_ = true;
    float dft = 1.0 / fps;
    float dut = 1.0 / gameConfig.ups;
    float accumulator = 0;
    unsigned int c_fps = 0;
    unsigned int c_ups = 0;
    float dt = 0;
    float waterDt = 0;
    while(!window_.shouldClose() && running_)
    {
        float elaps = m_timer.getElapsedTimeReset();   // Gets elapsed time since last clock reset and resets clock
        accumulator += elaps;
        
        while(accumulator >= dut) {
            update();
            accumulator -= dut;
            c_ups++;
        }
        float rdt = accumulator / dut;
        waterDt += elaps;
        waterDt = fmod(waterDt, 2*PI);
        render(rdt, waterDt);   // Don't use dt in rendering until we have way to deal with rotation interpolation
        c_fps++;

        window_.swapBuffers();

        dt += m_timer.getElapsedTime();
        if(dt > 1.0)
        {
            LOG_INFO("FPS:", c_fps, "UPS:", c_ups, world_->getPlayer());
            dt -= 1.0;
            c_fps = 0;
            c_ups = 0;
        }
    }
    cleanUp();
}

void Game::update() {
    //Timer("Updating");
    window_.pollEvents();
    world_->update();
}

void Game::render(float dt, float waterdt) {
    //Timer timer("Rendering");
    renderer_.render(world_, dt, waterdt);
}

void Game::close() {
    running_ = false;
    LOG_INFO("Set game running to false!");
}

void Game::cleanUp()
{
    renderer_.cleanUp();
    if(world_)
    {
        world_->cleanUp();
        delete world_;
    }
    LOG_INFO("Cleaned up game");
}

void Game::loadWorld(const std::string filename) {
   if (world_ != nullptr) delete world_;
   world_ = new World(filename);
}
