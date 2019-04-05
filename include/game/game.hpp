#pragma once

#include <string>

#include "world/world.hpp"
#include "game/window.hpp"
#include "game/renderer.hpp"
#include "game/event/key_type.hpp"
#include "util/timer.hpp"
#include "game/config.hpp"

class Game {
    public:
        Game();

        void keyCallback(bool* keysPressed);
        void mouseCallback(float dx, float dy);

        void init();
        void start();
        void update();
        void render(float dt, float waterdt);
        void close();
        void cleanUp();

        void loadWorld(const std::string filename);
    private:
        GameConfig gameConfig;
        unsigned int fps = 60;
        unsigned int ups = 60;
        World* world_ = nullptr;
        Window window_;
        Renderer renderer_;
        bool running_ = false;
};