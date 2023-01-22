#pragma once

#include "Mesh.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Renderable {
    GLuint vao = 0;
    GLuint indices = 0;

    void drawArrays(GLsizei count) const;
    void drawElements() const;
};

class VertexArray final {
  public:
    VertexArray();

    void bufferMesh(const Mesh& mesh);

    Renderable getRendable();

  private:
    void bufferVertexData(const std::vector<Vertex>& verts);
    void bufferIndicesData(const std::vector<GLuint> indices);

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ibo = 0;
    GLuint m_numIndices = 0;

  public:
    VertexArray& operator=(VertexArray&& other);
    VertexArray(VertexArray&& other);
    ~VertexArray();
    void reset();

    VertexArray& operator=(VertexArray& other) = delete;
    VertexArray(VertexArray& other) = delete;
};
