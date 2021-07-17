#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

constexpr int TERRAIN_SIZE = 128;

struct Vertex {
    glm::vec3 position{0.0f};
    glm::vec2 textureCoord{0.0f};
    glm::vec3 normal{0.0f};
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    GLsizei indicesCount = 0;
};

Mesh createQuadMesh();
Mesh createCubeMesh(const glm::vec3& dimensions);
Mesh createTerrainMesh(int size, int edgeVertices, bool isFlat);
