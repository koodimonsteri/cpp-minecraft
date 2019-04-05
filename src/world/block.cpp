#include "world/block.hpp"

void Block::setFacing(const Direction d) {
}

void Block::setRenderedFace(const Direction d, const bool value){
    switch (d) {
        case X_POS: blockData ^= (-value ^ blockData) & (1u << 29); break;
        case X_NEG: blockData ^= (-value ^ blockData) & (1u << 28); break;
        case Y_POS: blockData ^= (-value ^ blockData) & (1u << 27); break;
        case Y_NEG: blockData ^= (-value ^ blockData) & (1u << 26); break;
        case Z_POS: blockData ^= (-value ^ blockData) & (1u << 25); break;
        case Z_NEG: blockData ^= (-value ^ blockData) & (1u << 24); break;
        default: throw std::runtime_error("Invalid direction Block::setRenderedFace");
    }
}

void Block::setOcludedCorner(const Direction d, const Corner c, const bool value){
    switch (d) {
        case X_POS: {
            switch (c) {
                case X_POS_Y_NEG_Z_NEG: blockData ^= (-value ^ blockData) & (1u << 23); break;
                case X_POS_Y_NEG_Z_POS: blockData ^= (-value ^ blockData) & (1u << 22); break;
                case X_POS_Y_POS_Z_NEG: blockData ^= (-value ^ blockData) & (1u << 21); break;
                case X_POS_Y_POS_Z_POS: blockData ^= (-value ^ blockData) & (1u << 20); break;
                default: throw std::runtime_error("Invalid corner Block::setOcludedCorner");
            }
            break;
        };
        case X_NEG: {
            switch (c) {
                case X_NEG_Y_NEG_Z_POS: blockData ^= (-value ^ blockData) & (1u << 19); break;
                case X_NEG_Y_NEG_Z_NEG: blockData ^= (-value ^ blockData) & (1u << 18); break;
                case X_NEG_Y_POS_Z_POS: blockData ^= (-value ^ blockData) & (1u << 17); break;
                case X_NEG_Y_POS_Z_NEG: blockData ^= (-value ^ blockData) & (1u << 16); break;
                default: throw std::runtime_error("Invalid corner Block::setOcludedCorner");
            }
            break;
        };
        case Y_POS: {
            switch (c) {
                case X_POS_Y_POS_Z_NEG: blockData ^= (-value ^ blockData) & (1u << 15); break;
                case X_NEG_Y_POS_Z_NEG: blockData ^= (-value ^ blockData) & (1u << 14); break;
                case X_POS_Y_POS_Z_POS: blockData ^= (-value ^ blockData) & (1u << 13); break;
                case X_NEG_Y_POS_Z_POS: blockData ^= (-value ^ blockData) & (1u << 12); break;
                default: throw std::runtime_error("Invalid corner Block::setOcludedCorner");
            }
            break;
        };
        case Y_NEG: {
            switch (c) {
                case X_NEG_Y_NEG_Z_NEG: blockData ^= (-value ^ blockData) & (1u << 11); break;
                case X_POS_Y_NEG_Z_NEG: blockData ^= (-value ^ blockData) & (1u << 10); break;
                case X_NEG_Y_NEG_Z_POS: blockData ^= (-value ^ blockData) & (1u << 9 ); break;
                case X_POS_Y_NEG_Z_POS: blockData ^= (-value ^ blockData) & (1u << 8 ); break;
                default: throw std::runtime_error("Invalid corner Block::setOcludedCorner");
            }
            break;
        };
        case Z_POS: {
            switch (c) {
                case X_POS_Y_NEG_Z_POS: blockData ^= (-value ^ blockData) & (1u << 7); break;
                case X_NEG_Y_NEG_Z_POS: blockData ^= (-value ^ blockData) & (1u << 6); break;
                case X_POS_Y_POS_Z_POS: blockData ^= (-value ^ blockData) & (1u << 5); break;
                case X_NEG_Y_POS_Z_POS: blockData ^= (-value ^ blockData) & (1u << 4); break;
                default: throw std::runtime_error("Invalid corner Block::setOcludedCorner");
            }
            break;
        };
        case Z_NEG: {
            switch (c) {
                case X_NEG_Y_NEG_Z_NEG: blockData ^= (-value ^ blockData) & (1u << 3); break;
                case X_POS_Y_NEG_Z_NEG: blockData ^= (-value ^ blockData) & (1u << 2); break;
                case X_NEG_Y_POS_Z_NEG: blockData ^= (-value ^ blockData) & (1u << 1); break;
                case X_POS_Y_POS_Z_NEG: blockData ^= (-value ^ blockData) & (1u << 0); break;
                default: throw std::runtime_error("Invalid corner Block::setOcludedCorner");
            }
            break;
        };
        default: throw std::runtime_error("Invalid direction");
    }
}

FaceData Block::getFaceData(const Direction d) const {
    FaceData fd = DirectionUtil::getIndex(d);
    switch (d) {
        case X_POS: fd |= (blockData & 15 << 20) >> (20 - 4); break;
        case X_NEG: fd |= (blockData & 15 << 16) >> (16 - 4); break;
        case Y_POS: fd |= (blockData & 15 << 12) >> (12 - 4); break;
        case Y_NEG: fd |= (blockData & 15 << 8 ) >> (8  - 4); break;
        case Z_POS: fd |= (blockData & 15 << 4 ) >> (4  - 4); break;
        case Z_NEG: fd |= (blockData & 15 << 0 ) << (4); break;
        default: throw std::runtime_error("Invalid direction Block::setRenderedFace");
    }
    return fd;
}

bool Block::hasFace(const Direction d) const {
    return blockData & (d << BlockDataOffsets::RENDERED_FACES_OFFSET);
}

bool Block::isRendered() const {
    return blockData & (63 << BlockDataOffsets::RENDERED_FACES_OFFSET);
}