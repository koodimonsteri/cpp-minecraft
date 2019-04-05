#pragma once

#include <array>
#include "glm/glm.hpp"
#include "util/corner.hpp"

// 0 0 +x -x +y -y +z -z
enum Direction : unsigned char {
    X_POS = 1 << 5,
    X_NEG = 1 << 4,
    Y_POS = 1 << 3,
    Y_NEG = 1 << 2,
    Z_POS = 1 << 1,
    Z_NEG = 1
};

namespace DirectionUtil {
    static const Direction Directions[] = {X_POS, X_NEG, Y_POS, Y_NEG, Z_POS, Z_NEG};

    static glm::ivec3 directionToOffset(Direction d) {
        switch (d) {
            case X_POS: return glm::ivec3( 1, 0, 0);
            case X_NEG: return glm::ivec3(-1, 0, 0);
            case Y_POS: return glm::ivec3( 0, 1, 0);
            case Y_NEG: return glm::ivec3( 0,-1, 0);
            case Z_POS: return glm::ivec3( 0, 0, 1);
            case Z_NEG: return glm::ivec3( 0, 0,-1);
            default   : throw std::runtime_error("Invalid direction");
        }
    }

    static Direction getOpposite(Direction d) {
        switch (d) {
            case X_POS: return X_NEG;
            case X_NEG: return X_POS;
            case Y_POS: return Y_NEG;
            case Y_NEG: return Y_POS;
            case Z_POS: return Z_NEG;
            case Z_NEG: return Z_POS;
            default   : throw std::runtime_error("Invalid direction");
        }
    }

    static int getIndex(const Direction& d) {
        switch (d) {
            case X_POS: return 0;
            case X_NEG: return 1;
            case Y_POS: return 2;
            case Y_NEG: return 3;
            case Z_POS: return 4;
            case Z_NEG: return 5;
            default: throw std::runtime_error("Invalid direction");
        }
    }

    static std::array<Corner, 4> getCornersInDirection(const Direction& d) {
        switch (d) {
            case X_POS: return std::array<Corner, 4>{X_POS_Y_NEG_Z_NEG, X_POS_Y_NEG_Z_POS, X_POS_Y_POS_Z_NEG, X_POS_Y_POS_Z_POS};
            case X_NEG: return std::array<Corner, 4>{X_NEG_Y_NEG_Z_POS, X_NEG_Y_NEG_Z_NEG, X_NEG_Y_POS_Z_POS, X_NEG_Y_POS_Z_NEG};
            case Y_POS: return std::array<Corner, 4>{X_POS_Y_POS_Z_NEG, X_NEG_Y_POS_Z_NEG, X_POS_Y_POS_Z_POS, X_NEG_Y_POS_Z_POS};
            case Y_NEG: return std::array<Corner, 4>{X_NEG_Y_NEG_Z_NEG, X_POS_Y_NEG_Z_NEG, X_NEG_Y_NEG_Z_POS, X_POS_Y_NEG_Z_POS};
            case Z_POS: return std::array<Corner, 4>{X_POS_Y_NEG_Z_POS, X_NEG_Y_NEG_Z_POS, X_POS_Y_POS_Z_POS, X_NEG_Y_POS_Z_POS};
            case Z_NEG: return std::array<Corner, 4>{X_NEG_Y_NEG_Z_NEG, X_POS_Y_NEG_Z_NEG, X_NEG_Y_POS_Z_NEG, X_POS_Y_POS_Z_NEG};
            default: throw std::runtime_error("Invalid direction");
        }
    }

    static std::array<glm::tvec3<char>, 3> getAoOffset(const Direction& d, int vertexId) {
        switch (d) {
            case X_POS: {
                switch (vertexId) {
                    case 0: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(1,  0, -1), glm::tvec3<char>(1, -1,  0), glm::tvec3<char>(1, -1, -1)};
                    case 1: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(1, -1,  0), glm::tvec3<char>(1,  0,  1), glm::tvec3<char>(1, -1,  1)};
                    case 3: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(1,  0,  1), glm::tvec3<char>(1,  1,  0), glm::tvec3<char>(1,  1,  1)};
                    case 2: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(1,  1,  0), glm::tvec3<char>(1,  0, -1), glm::tvec3<char>(1,  1, -1)};
                }
            };
            case X_NEG: {
                switch (vertexId) {
                    case 0: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1,  0,  1), glm::tvec3<char>(-1, -1,  0), glm::tvec3<char>(-1, -1,  1)};
                    case 1: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1, -1,  0), glm::tvec3<char>(-1,  0, -1), glm::tvec3<char>(-1, -1, -1)};
                    case 3: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1,  0, -1), glm::tvec3<char>(-1,  1,  0), glm::tvec3<char>(-1,  1, -1)};
                    case 2: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1,  1,  0), glm::tvec3<char>(-1,  0,  1), glm::tvec3<char>(-1,  1,  1)};
                }
            };
            case Y_POS: {
                switch (vertexId) {
                    case 0: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 1, 1,  0), glm::tvec3<char>( 0, 1,  1), glm::tvec3<char>( 1, 1,  1)};
                    case 1: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0, 1,  1), glm::tvec3<char>(-1, 1,  0), glm::tvec3<char>(-1, 1,  1)};
                    case 3: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1, 1,  0), glm::tvec3<char>( 0, 1, -1), glm::tvec3<char>(-1, 1, -1)};
                    case 2: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0, 1, -1), glm::tvec3<char>( 1, 1,  0), glm::tvec3<char>( 1, 1, -1)};
                }
            };
            case Y_NEG: {
                switch (vertexId) {
                    case 0: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1, -1,  0), glm::tvec3<char>( 0, -1,  1), glm::tvec3<char>(-1, -1,  1)};
                    case 1: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0, -1,  1), glm::tvec3<char>( 1, -1,  0), glm::tvec3<char>( 1, -1,  1)};
                    case 3: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 1, -1,  0), glm::tvec3<char>( 0, -1, -1), glm::tvec3<char>( 1, -1, -1)};
                    case 2: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0, -1, -1), glm::tvec3<char>(-1, -1,  0), glm::tvec3<char>(-1, -1, -1)};
                }
            };
            case Z_POS: {
                switch (vertexId) {
                    case 0: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 1,  0, 1), glm::tvec3<char>( 0, -1, 1), glm::tvec3<char>( 1, -1, 1)};
                    case 1: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0, -1, 1), glm::tvec3<char>(-1,  0, 1), glm::tvec3<char>(-1, -1, 1)};
                    case 3: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1,  0, 1), glm::tvec3<char>( 0,  1, 1), glm::tvec3<char>(-1,  1, 1)};
                    case 2: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0,  1, 1), glm::tvec3<char>( 1,  0, 1), glm::tvec3<char>( 1,  1, 1)};
                }
            };
            case Z_NEG: {
                switch (vertexId) {
                    case 0: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1,  0, -1), glm::tvec3<char>( 0, -1, -1), glm::tvec3<char>(-1, -1, -1)};
                    case 1: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0, -1, -1), glm::tvec3<char>( 1,  0, -1), glm::tvec3<char>( 1, -1, -1)};
                    case 3: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 1,  0, -1), glm::tvec3<char>( 0,  1, -1), glm::tvec3<char>( 1,  1, -1)};
                    case 2: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0,  1, -1), glm::tvec3<char>(-1,  0, -1), glm::tvec3<char>(-1,  1, -1)};
                }
            };
            default: throw std::runtime_error("Invalid direction");
        }
    }

        static std::array<glm::tvec3<char>, 3> getAoOffsets(const Direction& d, const Corner& c) {
        switch (d) {
            case X_POS: {
                switch (c) {
                    case X_POS_Y_NEG_Z_NEG: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(1,  0, -1), glm::tvec3<char>(1, -1,  0), glm::tvec3<char>(1, -1, -1)};
                    case X_POS_Y_NEG_Z_POS: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(1, -1,  0), glm::tvec3<char>(1,  0,  1), glm::tvec3<char>(1, -1,  1)};
                    case X_POS_Y_POS_Z_NEG: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(1,  1,  0), glm::tvec3<char>(1,  0, -1), glm::tvec3<char>(1,  1, -1)};
                    case X_POS_Y_POS_Z_POS: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(1,  0,  1), glm::tvec3<char>(1,  1,  0), glm::tvec3<char>(1,  1,  1)};
                    default: throw std::runtime_error("Invalid corner");
                }
            };
            case X_NEG: {
                switch (c) {
                    case X_NEG_Y_NEG_Z_POS: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1,  0,  1), glm::tvec3<char>(-1, -1,  0), glm::tvec3<char>(-1, -1,  1)};
                    case X_NEG_Y_NEG_Z_NEG: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1, -1,  0), glm::tvec3<char>(-1,  0, -1), glm::tvec3<char>(-1, -1, -1)};
                    case X_NEG_Y_POS_Z_POS: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1,  1,  0), glm::tvec3<char>(-1,  0,  1), glm::tvec3<char>(-1,  1,  1)};
                    case X_NEG_Y_POS_Z_NEG: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1,  0, -1), glm::tvec3<char>(-1,  1,  0), glm::tvec3<char>(-1,  1, -1)};
                    default: throw std::runtime_error("Invalid corner");
                }
            };
            case Y_POS: {
                switch (c) {
                    case X_POS_Y_POS_Z_NEG: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 1, 1,  0), glm::tvec3<char>( 0, 1,  1), glm::tvec3<char>( 1, 1,  1)};
                    case X_NEG_Y_POS_Z_NEG: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0, 1,  1), glm::tvec3<char>(-1, 1,  0), glm::tvec3<char>(-1, 1,  1)};
                    case X_POS_Y_POS_Z_POS: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0, 1, -1), glm::tvec3<char>( 1, 1,  0), glm::tvec3<char>( 1, 1, -1)};
                    case X_NEG_Y_POS_Z_POS: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1, 1,  0), glm::tvec3<char>( 0, 1, -1), glm::tvec3<char>(-1, 1, -1)};
                    default: throw std::runtime_error("Invalid corner");
                }
            };
            case Y_NEG: {
                switch (c) {
                    case X_NEG_Y_NEG_Z_NEG: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1, -1,  0), glm::tvec3<char>( 0, -1,  1), glm::tvec3<char>(-1, -1,  1)};
                    case X_POS_Y_NEG_Z_NEG: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0, -1,  1), glm::tvec3<char>( 1, -1,  0), glm::tvec3<char>( 1, -1,  1)};
                    case X_NEG_Y_NEG_Z_POS: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0, -1, -1), glm::tvec3<char>(-1, -1,  0), glm::tvec3<char>(-1, -1, -1)};
                    case X_POS_Y_NEG_Z_POS: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 1, -1,  0), glm::tvec3<char>( 0, -1, -1), glm::tvec3<char>( 1, -1, -1)};
                    default: throw std::runtime_error("Invalid corner");
                }
            };
            case Z_POS: {
                switch (c) {
                    case X_POS_Y_NEG_Z_POS: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 1,  0, 1), glm::tvec3<char>( 0, -1, 1), glm::tvec3<char>( 1, -1, 1)};
                    case X_NEG_Y_NEG_Z_POS: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0, -1, 1), glm::tvec3<char>(-1,  0, 1), glm::tvec3<char>(-1, -1, 1)};
                    case X_POS_Y_POS_Z_POS: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0,  1, 1), glm::tvec3<char>( 1,  0, 1), glm::tvec3<char>( 1,  1, 1)};
                    case X_NEG_Y_POS_Z_POS: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1,  0, 1), glm::tvec3<char>( 0,  1, 1), glm::tvec3<char>(-1,  1, 1)};
                    default: throw std::runtime_error("Invalid corner");
                }
            };
            case Z_NEG: {
                switch (c) {
                    case X_NEG_Y_NEG_Z_NEG: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>(-1,  0, -1), glm::tvec3<char>( 0, -1, -1), glm::tvec3<char>(-1, -1, -1)};
                    case X_POS_Y_NEG_Z_NEG: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0, -1, -1), glm::tvec3<char>( 1,  0, -1), glm::tvec3<char>( 1, -1, -1)};
                    case X_NEG_Y_POS_Z_NEG: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 0,  1, -1), glm::tvec3<char>(-1,  0, -1), glm::tvec3<char>(-1,  1, -1)};
                    case X_POS_Y_POS_Z_NEG: return std::array<glm::tvec3<char>, 3>{glm::tvec3<char>( 1,  0, -1), glm::tvec3<char>( 0,  1, -1), glm::tvec3<char>( 1,  1, -1)};
                    default: throw std::runtime_error("Invalid corner");
                }
            };
            default: throw std::runtime_error("Invalid direction");
        }
    }
}