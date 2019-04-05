#include "util/texture_atlas.hpp"

// STL
#include <array>

TextureAtlas::TextureAtlas(PngImage& image, int rows, int cols, int texSlot) : image_(image), rows_(rows), cols_(cols), textureSlot(texSlot) {}

void TextureAtlas::init(GLuint& tex) {
    int w = image_.getWidth()/cols_;
    int h = image_.getHeight()/rows_;

    GLuint texture;

    glGenTextures(1,&tex);
    glActiveTexture(GL_TEXTURE0 + textureSlot);   ///////////

    glBindTexture(GL_TEXTURE_2D_ARRAY,tex);

    // Allocate the storage.
    glTexImage3D(GL_TEXTURE_2D_ARRAY,
                 0,                 // level
                 GL_RGBA,           // Internal format
                 w, h, rows_*cols_,          // width,height,depth
                 0,                 // border?
                 GL_RGBA,           // format
                 GL_UNSIGNED_BYTE,  // type
                 0);                // pointer to data

    load();

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    // Always set reasonable texture parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void TextureAtlas::load() {
    int img_w = image_.getWidth();
    int img_h = image_.getHeight();
    int sub_w = img_w/cols_;
    int sub_h = img_h/rows_;

    int col = 0;
    int row = 0;

    int textureId = 0;
    for (auto row = 0u; row < rows_; ++row) {
        for (auto col = 0u; col < cols_; ++col) {
            char* subTexture = new char[4*sub_w*sub_h];
            int sub_i = 0;
            for (auto sub_y = 0u; sub_y < sub_h; ++sub_y) {
                for (auto sub_x = 0u; sub_x < sub_w; ++sub_x) {
                    int img_x = col*sub_w + sub_x;
                    int img_y = (rows_ - row - 1)*sub_h + sub_y;
                    
                    int img_i = img_w*img_y + img_x;

                    subTexture[4*sub_i + 0] = image_.getData()[4*img_i + 0];
                    subTexture[4*sub_i + 1] = image_.getData()[4*img_i + 1];
                    subTexture[4*sub_i + 2] = image_.getData()[4*img_i + 2];
                    subTexture[4*sub_i + 3] = image_.getData()[4*img_i + 3];
                    ++sub_i;
                }
            }

            glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                            0,                 // Mipmap number
                            0, 0, textureId,   // xoffset, yoffset, zoffset
                            sub_w, sub_h, 1,   // width, height, depth
                            GL_RGBA,           // format
                            GL_UNSIGNED_BYTE,  // type
                            subTexture);       // pointer to data

            delete[] subTexture;
            ++textureId;
        }
    }
}
