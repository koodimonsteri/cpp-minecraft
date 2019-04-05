#pragma once

// STL
#include <string>
#include <vector>

// 3rd party
#include "glad/glad.h"

class PngImage {
    public:
        PngImage(const std::string filename);
        const GLubyte* getData() const;
        int getWidth() const { return width_; };
        int getHeight() const { return height_; };
    private:
        int width_;
        int height_;
        int bpp_;
        GLubyte* data_ = nullptr; 
};