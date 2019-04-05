#include "util/png_image.hpp"

// STL
#include <iostream>

// 3rd party
#include "stb_image.h"
#include "glad/glad.h"

PngImage::PngImage(const std::string filename) {
    stbi_set_flip_vertically_on_load(true);
    data_ = stbi_load(filename.c_str(), &width_, &height_, &bpp_, 4);
    if (!data_) {
        std::cout << "vituiks män" << std::endl;
    }
}

const GLubyte* PngImage::getData() const {
    return data_;
}