#pragma once

#include <glm/glm.hpp>

class World;

class Player {
    public:
        Player(World& world);

        void update();
        void keyCallback(bool* keysPressed);
        void mouseCallback(float dx, float dy);

        void punch() const;

        const glm::vec3& getPosition() const;
        const glm::vec3& getFacing() const;
        const glm::vec3& getVelocity() const; 
        glm::vec3        getCurrentFacing();
    private:
        const float PLAYER_HEIGHT = 1.8f;
        glm::vec3 pos_ = glm::vec3(5,60,-5);
        glm::vec3 vel_ = glm::vec3(0,0,0);
        glm::vec3 facing_ = glm::vec3(0,0,1);
        
        World& world_;
};
