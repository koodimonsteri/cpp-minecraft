#pragma once

#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "util/bounding_box.hpp"

enum RenderableType {
    EMPTY, SOLID, UNSOLID, FLOWER, SKYBOX
};

struct Renderable {
    GLuint vao;
    GLsizei size;
    glm::mat4 model;
    BoundingBox bb;
    RenderableType type;
};

// Container to hold different types of renderable
struct RenderableContainer {
    BoundingBox             chunkBB;
    std::vector<Renderable> renderables;
};