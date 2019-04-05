#pragma once

struct GameConfig {
    int   windowWidth    = 1000;
    int   windowHeight   = 720;
    bool  isFullScreen   = false;
    int   seed           = 10000;  // Seed of the world
    float fov            = 90.0;   // Field of view
    float renderDistance = 11;     // Renderdistance in chunks
    bool  vSync          = true;
    unsigned int fps     = 60;     // Frames per second
    unsigned int ups     = 60;     // Updates per second
};