#pragma once

#include <array>

#include "glm/glm.hpp"

enum Corner : unsigned char {
    X_NEG_Y_NEG_Z_NEG = 1,      // 00 00 00 01
    X_NEG_Y_NEG_Z_POS = 1 << 1, // 00 00 00 10
    X_NEG_Y_POS_Z_NEG = 1 << 2, // 00 00 01 00
    X_NEG_Y_POS_Z_POS = 1 << 3, // 00 00 10 00
    X_POS_Y_NEG_Z_NEG = 1 << 4, // 00 01 00 00
    X_POS_Y_NEG_Z_POS = 1 << 5, // 00 10 00 00
    X_POS_Y_POS_Z_NEG = 1 << 6, // 01 00 00 00
    X_POS_Y_POS_Z_POS = 1 << 7  // 10 00 00 00
};