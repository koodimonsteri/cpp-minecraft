#pragma once

#include "util/renderable.hpp"

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include <array>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 nor;
    glm::vec2 tex;
    void translate(const glm::vec3& translation);
};

class Mesh {
    public:
        Mesh() {}
        Renderable getRenderable() const;

        const std::vector<Vertex>& getVertices() const;
        void push_back(const std::vector<Vertex>& vertices);
        void push_back(const std::array<Vertex, 6>& vertices);
        void translate(const glm::vec3& translation);
    private:
        std::vector<Vertex> vertices_;
};