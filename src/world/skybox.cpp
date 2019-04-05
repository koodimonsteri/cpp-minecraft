#include "world/skybox.hpp"
#include "util/log.hpp"

#include <math.h>
#include <vector>

SkyBox::SkyBox() {
    //auto temp = Game::getGameConfig();
    //scale_ = ((temp.renderDistance * 16) / sqrt(2)) - 20;
    LOG_INFO("SKYBOX SCALE", scale_);
}

SkyBox::~SkyBox() {}

void SkyBox::init()
{
    GLfloat vertexCoords[] = {
        // Left
        -scale_, -scale_, -scale_,
        -scale_,  scale_, -scale_,
        -scale_, -scale_,  scale_,
        -scale_, -scale_,  scale_,
        -scale_,  scale_, -scale_,
        -scale_,  scale_,  scale_,
        // Right
         scale_, -scale_,  scale_,
         scale_,  scale_,  scale_,
         scale_, -scale_, -scale_,
         scale_, -scale_, -scale_,
         scale_,  scale_,  scale_,
         scale_,  scale_, -scale_,
        // Front
        -scale_, -scale_,  scale_,
        -scale_,  scale_,  scale_,
         scale_, -scale_,  scale_,
         scale_, -scale_,  scale_,
        -scale_,  scale_,  scale_,
         scale_,  scale_,  scale_,
        // Back
         scale_, -scale_, -scale_,
         scale_,  scale_, -scale_,
        -scale_, -scale_, -scale_,
        -scale_, -scale_, -scale_,
         scale_,  scale_, -scale_,
        -scale_,  scale_, -scale_,
        // Top
         scale_,  scale_, -scale_,
         scale_,  scale_,  scale_,
        -scale_,  scale_, -scale_,
        -scale_,  scale_, -scale_,
         scale_,  scale_,  scale_,
        -scale_,  scale_,  scale_,
        // Bottom
         scale_, -scale_,  scale_,
         scale_, -scale_, -scale_,
        -scale_, -scale_,  scale_,
        -scale_, -scale_,  scale_,
         scale_, -scale_, -scale_,
        -scale_, -scale_, -scale_,
    };

    GLfloat texCoords[] = {
        // Left
        0, 0, 0,
        0, 1, 0,
        1, 0, 0,
        1, 0, 0,
        0, 1, 0,
        1, 1, 0,
        // Right
        0, 0, 2,
        0, 1, 2,
        1, 0, 2,
        1, 0, 2,
        0, 1, 2,
        1, 1, 2,
        // Front
        0, 0, 1,
        0, 1, 1,
        1, 0, 1,
        1, 0, 1,
        0, 1, 1,
        1, 1, 1,
        // Back
        0, 0, 3,
        0, 1, 3,
        1, 0, 3,
        1, 0, 3,
        0, 1, 3,
        1, 1, 3,
        // Top
        1, 0, 4,
        0.004, 0, 4,      // Offset top face x coordinate by tiny amount to prevent texture tearing
        1, 1, 4,
        1, 1, 4,
        0.004, 0, 4,
        0.004, 1, 4,
        // Bottom
        0, 0, 5,
        0, 1, 5,
        1, 0, 5,
        1, 0, 5,
        0, 1, 5,
        1, 1, 5,
    };

    renderable_.type = SKYBOX;
    renderable_.size = 36*3;  // 36 vertices * 3 floats per vertex
    GLuint vbos[2];

    glGenVertexArrays(1, &renderable_.vao);
    glBindVertexArray(renderable_.vao);

    glGenBuffers(2, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, renderable_.size * sizeof(GLfloat), &vertexCoords[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, renderable_.size * sizeof(GLfloat), &texCoords[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    LOG_INFO("Initialized SkyBox");
}

void SkyBox::setScale(float s)
{
    scale_ = s;
}

Renderable SkyBox::getRenderable() const
{
    return renderable_;
}