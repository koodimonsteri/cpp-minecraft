#include "game/player.hpp"

// STL
#include <iostream>
#include <functional>
#include <algorithm>

// 3rd party
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

// Own
#include "game/event/event_util.hpp"
#include "game/event/event_handler.hpp"
#include "game/event/key_type.hpp"
#include "game/game.hpp"
#include "util/math_constants.hpp"
#include "util/log.hpp"
#include "world/world.hpp"

Player::Player(World& world) : world_(world) {
    using namespace std::placeholders;
    EventHandler<bool*>::appendAction(EventType::E_KEY, std::bind(&Player::keyCallback, this, _1));
    EventHandler<float, float>::appendAction(EventType::E_MOUSE, std::bind(&Player::mouseCallback, this, _1, _2));
}

void Player::keyCallback(bool* keysPressed) {
    // Update vel_
    glm::vec3 localX = normalize(cross(facing_, UP));
    glm::vec3 vel = glm::vec3(0);

    if (keysPressed[KEY_W]) vel += facing_;
    if (keysPressed[KEY_A]) vel -= localX;
    if (keysPressed[KEY_S]) vel -= facing_;
    if (keysPressed[KEY_D]) vel += localX;
    if (keysPressed[KEY_SPACE]) vel += UP;
    if (keysPressed[KEY_LEFT_SHIFT]) vel -= UP;

    vel_ = length(vel) > 0.0001 ? normalize(vel) : vel;
}

void Player::mouseCallback(float dx, float dy) {
    static float yaw = atan2(facing_.z, facing_.x);
    static float pitch = asin(facing_.y);
    static const float sensitivity = 1.0f/1000.0f;

    yaw = std::fmod(yaw + dx*sensitivity, 2*PI);
    pitch = std::clamp(pitch - dy*sensitivity, -PI_2 + EPSILON, PI_2 - EPSILON);
    float scale = std::cos(pitch);

    facing_ = glm::vec3(scale*std::cos(yaw), std::sin(pitch), scale*std::sin(yaw));

    //rotVec_ += glm::vec2(dx, dy);
    //static int rotCount = 0;
    //LOG_INFO(rotVec_, rotCount++);
}

void Player::update() {
    //updateFacing();
    static const float maxSpeed = 20.0f/100.0f;
    pos_ += vel_*maxSpeed;
}

void Player::punch() const {
    Ray r = {facing_, pos_};
    auto intersection = world_.intersectsRay(r);
    if (intersection) {
        world_.destroyBlock(intersection->blockPos);
    }
}

const glm::vec3& Player::getPosition() const {
    return pos_;
}

const glm::vec3& Player::getFacing() const {
    return facing_;
}

const glm::vec3& Player::getVelocity() const {
    return vel_;
}