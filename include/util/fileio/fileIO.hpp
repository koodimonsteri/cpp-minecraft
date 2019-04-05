#pragma once

#include <fstream>

#include "game/config.hpp"
#include "util/log.hpp"

#define MAXLINE 80

namespace FileIO {

    GameConfig loadGameConfig(const std::string& path);
    void       writeGameConfig(const GameConfig& gc, const std::string& path);

    void       initWorldDirectory(const std::string& worldName);
}