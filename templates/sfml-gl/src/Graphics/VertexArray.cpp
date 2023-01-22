#include "VertexArray.h"
#include <glm/glm.hpp>
#include <iostream>

VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &m_vao);
    glCreateBuffers(1, &m_vbo);
    glCreateBuffers(1, &m_ibo);
}

VertexArray& VertexArray::operator=(VertexArray&& other)
{
    m_vao = other.m_vao;
    m_vbo = other.m_vbo;
    m_ibo = other.m_ibo;
    m_numIndices = other.m_numIndices;

    other.m_vao = 0;
    other.m_vbo = 0;
    other.m_ibo = 0;
    other.m_numIndices = 0;

    return *this;
}

VertexArray::VertexArray(VertexArray&& other)
    : m_vao{other.m_vao}
    , m_vbo{other.m_vbo}
    , m_ibo{other.m_ibo}
    , m_numIndices{other.m_numIndices}
{
    other.m_vao = 0;
    other.m_vbo = 0;
    other.m_ibo = 0;
    other.m_numIndices = 0;
}

VertexArray::~VertexArray()
{
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ibo);
    }
}

void VertexArray::reset()
{
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ibo);
    }
    glCreateVertexArrays(1, &m_vao);
    glCreateBuffers(1, &m_vbo);
    glCreateBuffers(1, &m_ibo);
    m_numIndices = 0;
}

void VertexArray::bufferVertexData(const std::vector<Vertex>& verts)
{
    // glBufferData
    glNamedBufferStorage(m_vbo, sizeof(Vertex) * verts.size(), verts.data(), GL_DYNAMIC_STORAGE_BIT);

    // Attach the vertex array to the vertex buffer and element buffer
    glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(Vertex));

    // glEnableVertexAttribArray
    glEnableVertexArrayAttrib(m_vao, 0);
    glEnableVertexArrayAttrib(m_vao, 1);
    glEnableVertexArrayAttrib(m_vao, 2);

    // glVertexAttribPointer
    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, textureCoord));
    glVertexArrayAttribFormat(m_vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
    glVertexArrayAttribBinding(m_vao, 0, 0);
    glVertexArrayAttribBinding(m_vao, 1, 0);
    glVertexArrayAttribBinding(m_vao, 2, 0);
}

void VertexArray::bufferIndicesData(const std::vector<GLuint> indices)
{
    glNamedBufferStorage(m_ibo, sizeof(GLuint) * indices.size(), indices.data(), GL_DYNAMIC_STORAGE_BIT);
    glVertexArrayElementBuffer(m_vao, m_ibo);
    m_numIndices = indices.size();
}

void VertexArray::bufferMesh(const Mesh& mesh)
{
    bufferVertexData(mesh.vertices);
    bufferIndicesData(mesh.indices);
}

Renderable VertexArray::getRendable()
{
    return {m_vao, m_numIndices};
}

void Renderable::drawArrays(GLsizei count) const
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, count);
}

void Renderable::drawElements() const
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr);
}
