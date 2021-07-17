#pragma once

#include <glad/glad.h>

GLuint loadFromFile(const char* file);
GLuint createFramebufferTexture(GLint width, GLint height);

class Texture2D final {
  public:
    Texture2D();

    void bind(GLenum unit) const;

    void loadFromFile(const char* file, int mipmapLevels);
    void createFramebufferTexture(GLint width, GLint height);
    void createFramebufferDepth(GLint width, GLint height);

    void wrapS(GLint param);
    void wrapT(GLint param);
    void minFilter(GLint param);
    void magFilter(GLint param);
    void useDefaultFilters();

  private:
    GLuint m_handle = 0;

    friend class Framebuffer;

  public:
    Texture2D& operator=(Texture2D&& other) noexcept;
    Texture2D(Texture2D&& other) noexcept;
    ~Texture2D();
    Texture2D& operator=(Texture2D& other) = delete;
    Texture2D(Texture2D& other) = delete;
};

class TextureArray2D {
  public:
    TextureArray2D();

    void create(GLuint textureDims, GLuint textureCount);
    GLuint addTexture(const char* file);

    void bind() const;

  private:
    GLuint m_handle = 0;
    GLuint m_textureDims = 0;
    GLuint m_textureCount = 0;
    GLuint m_maxTextures = 0;

  public:
    TextureArray2D& operator=(TextureArray2D&& other) noexcept;
    TextureArray2D(TextureArray2D&& other) noexcept;
    ~TextureArray2D();
    TextureArray2D& operator=(TextureArray2D& other) = delete;
    TextureArray2D(TextureArray2D& other) = delete;
};