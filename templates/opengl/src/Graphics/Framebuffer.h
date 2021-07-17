#pragma once

#include "Texture.h"
#include <glad/glad.h>
#include <vector>

class Framebuffer final {
  public:
    Framebuffer();

    void create(GLuint width, GLuint height);

    const Texture2D* addTexture();
    const Texture2D* addDepthTexture();
    void addRenderBuffer();
    void finish();

    void bind() const;

    static void unbind();

  private:
    std::vector<Texture2D> m_attachments;
    GLuint m_width = 0;
    GLuint m_height = 0;
    GLuint m_rbo = 0;
    GLuint m_fbo = 0;

  public:
    Framebuffer& operator=(Framebuffer&& other) noexcept;
    Framebuffer(Framebuffer&& other) noexcept;
    ~Framebuffer();

    Framebuffer& operator=(Framebuffer& other) = delete;
    Framebuffer(Framebuffer& other) = delete;
};