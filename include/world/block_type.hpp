#pragma once

// STL
#include <string>

// Own
#include "util/direction.hpp"

enum BlockType : unsigned char {
    DIRT,
    GRASS,
    STONE,
    TNT,
    WATER,
    AIR,
    NUM_BLOCK_TYPES
};

namespace BlockUtil {
    static unsigned char textureIds[6*NUM_BLOCK_TYPES] = {
    // +x -x +y -y +z -z
        2, 2, 2,  2, 2, 2, // DIRT
        3, 3, 0,  2, 3, 3, // GRASS
        1, 1, 1,  1, 1, 1, // STONE
        8, 8, 9, 10, 8, 8, // TNT
        14, 14, 14, 14, 14, 14,  // WATER
    };

    static unsigned char getTextureId(const BlockType& type, const Direction& d) {
        return textureIds[6*(type) + DirectionUtil::getIndex(d)];
    };

    static std::string toString(const BlockType& type) {
        switch (type) {
            case DIRT  : return std::string("DIRT");
            case GRASS : return std::string("GRASS");
            case STONE : return std::string("STONE");
            case TNT   : return std::string("TNT");
            case AIR   : return std::string("AIR");
            case WATER : return std::string("WATER");
            default    : return std::string("???");
        }
    }
}