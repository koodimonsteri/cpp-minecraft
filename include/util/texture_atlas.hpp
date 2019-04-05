#pragma once

// STL
#include <string>
#include <memory>

// 3rd party
#include "glad/glad.h"

// Own
#include "util/png_image.hpp"

class TextureAtlas {
    public:
        TextureAtlas(PngImage& image, int rows, int cols, int texSlot);
        void init(GLuint& tex);

    private:
        void load();
        const PngImage& image_;
        int rows_;
        int cols_;
        int textureSlot;
};