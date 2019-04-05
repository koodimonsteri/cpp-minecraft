#include "util/mesh.hpp"

#include <iostream>

void Vertex::translate(const glm::vec3& translation) {
    pos += translation;
}

Renderable Mesh::getRenderable() const {
    Renderable renderable;
    renderable.size = vertices_.size();
    GLuint vbos[3];

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

    for (auto &vertex : vertices_) {
        positions.push_back(vertex.pos);
        normals.push_back(vertex.nor);
        texCoords.push_back(vertex.tex);
    }

    glGenVertexArrays(1, &renderable.vao);
    glBindVertexArray(renderable.vao);

    glGenBuffers(3, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3), &positions.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, 0);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(glm::vec2), &texCoords.front(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return renderable;
}

const std::vector<Vertex>& Mesh::getVertices() const {
    return vertices_;
}

void Mesh::push_back(const std::vector<Vertex>& vertices) {
    for (auto &vertex : vertices) {
        vertices_.push_back(vertex);
    }
}

void Mesh::push_back(const std::array<Vertex, 6>& vertices) {
    for (auto &vertex : vertices) {
        vertices_.push_back(vertex);
    }
}

void Mesh::translate(const glm::vec3& translation) {
    for (auto &vertex : vertices_) {
        vertex.translate(translation);
    }
}