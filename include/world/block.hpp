#pragma once

// Own
#include "world/block_type.hpp"
#include "util/direction.hpp"
#include "util/corner.hpp"

// BlockData (32 bits) provides additional block information which
// will speed up the chunk mesh building. It keeps track
// of three kinds of attributes:
//
// # Facing (2 Bits [0, 1])
// Facing tells in what orientataion the block should be
// rendered. Rotations are done around the Y axis.
//
// 00 -> no rotation
// 01 -> 90°  rotation clockwise
// 10 -> 180° rotation
// 11 -> 90°  rotation counter clockwise
//
//
// # Rendered faces (6 Bits [2,7])
// Tells which faces should be rendered. Each bit represents
// whether a face in a particular direction should
// be rendered or not.
//
// Ordering:
// X_POS, X_NEG, Y_POS, Y_NEG, Z_POS, Z_NEG
// 
//
// # Ambient oclusion information (24 Bits [8, 31])
// Tells how the faces should be shaded. Each bit represent 
// wheter a corner of a particular face is ocluded or not.
//
// Ordering:
// X_POS: Bits [ 8, 11]
// X_NEG: Bits [12, 15]
// Y_POS: Bits [16, 19]
// Y_NEG: Bits [20, 23]
// Z_POS: Bits [24, 27]
// Z_NEG: Bits [28, 31]
// 
// Ordering of the four bits reserved for each face:
// v0, v1, v2, v3
//
// Vertices of a face
// v3---v2
// |     |
// v1---v0
//
typedef unsigned BlockData;

// FaceData (8 bits) is built from BlockData. It contains
// only the necessary information needed for 
// rendering a single face whereas BlockData
// contains information about all of the faces.
// FaceData holds two kinds of attributes.
//
// # Direction index (3 Bits [5,7])
// Direction index tells the direction of the
// face. 
//
// # Ambient Oclusion information (4 Bits [0,3])
// Each bit represent wheter a corner of the face
// is ocluded or not.
//
typedef char FaceData;

enum BlockDataOffsets {
    FACING_OFFSET = 30,
    RENDERED_FACES_OFFSET = 24,
    AO_OFFSET = 0,
};

struct Block {
    BlockType blockType = AIR;
    BlockData  blockData = 0;

    void setFacing(const Direction d);
    void setRenderedFace(const Direction d, const bool value);
    void setOcludedCorner(const Direction d, const Corner c, bool value);
    bool hasFace(const Direction d) const;
    bool isRendered() const;
    FaceData getFaceData(const Direction d) const;
};