#pragma once

// STL
#include <unordered_map>

// 3rd party
#include "glm/glm.hpp"

// Equals function for ChunkMap
struct equals {
    bool operator() (const glm::ivec2& v1, const glm::ivec2& v2) const {
        return (v1.x == v2.x) && (v1.y == v2.y);
    }
};

// Hash function for ChunkMap, joins x and y bits to 1 unsigned long long
struct hash {
    size_t operator()(const glm::ivec2& v) const
    {
        return size_t(((unsigned long long)v.x << 32) | v.y);
    }
};

template <typename T>
using ivec2_map = std::unordered_map<glm::ivec2, T, hash, equals>;