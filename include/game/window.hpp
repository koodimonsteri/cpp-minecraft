#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game/config.hpp"

class Window {
    public:
        Window(GameConfig& gc): gameConfig(gc) {};
        ~Window();
        void init();
        bool shouldClose() const;
        void pollEvents() const;
        void swapBuffers() const;
        void setVSync(bool vsync);
        void toggleCursor();
        void toggleFullScreen();

    private:
        GLFWwindow*  windowHandle_  = nullptr;
        GLFWmonitor* monitorHandle_ = nullptr;
        GameConfig&  gameConfig;
        bool         cursorEnabled = false;

        // These values are used to restore old window position and size after fullscreen mode
        int          windowPos[2]  = { 0, 0 };
        int          windowSize[2] = { 0, 0 };
};