#include "Texture.h"

#include <SFML/Graphics/Image.hpp>
#include <cstring>
#include <iostream>

Texture2D ::Texture2D()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
}

Texture2D& Texture2D ::operator=(Texture2D&& other) noexcept
{
    m_handle = other.m_handle;
    other.m_handle = 0;
    return *this;
}

Texture2D ::Texture2D(Texture2D&& other) noexcept
    : m_handle{other.m_handle}
{
    other.m_handle = 0;
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_handle);
}

void Texture2D::loadFromFile(const char* file, int mipmapLevels)
{
    char texturePath[128] = "Data/Textures/";
    std::strcat(texturePath, file);

    sf::Image img;
    img.loadFromFile(texturePath);

    int width = img.getSize().x;
    int height = img.getSize().y;

    glTextureStorage2D(m_handle, mipmapLevels, GL_RGBA8, width, height);
    glTextureSubImage2D(m_handle, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
    useDefaultFilters();
}

void Texture2D::createFramebufferTexture(GLint width, GLint height)
{
    glTextureStorage2D(m_handle, 1, GL_RGB8, width, height);
    useDefaultFilters();
}

void Texture2D::createFramebufferDepth(GLint width, GLint height)
{
    glTextureStorage2D(m_handle, 1, GL_DEPTH_COMPONENT24, width, height);
    useDefaultFilters();
}

void Texture2D::bind(GLenum unit) const
{
    glBindTextureUnit(unit, m_handle);
}

void Texture2D::wrapS(GLint param)
{
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, param);
}
void Texture2D::wrapT(GLint param)
{
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, param);
}
void Texture2D::minFilter(GLint param)
{
    glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, param);
}
void Texture2D::magFilter(GLint param)
{
    glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, param);
}

void Texture2D::useDefaultFilters()
{
    glGenerateTextureMipmap(m_handle);

    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameterf(m_handle, GL_TEXTURE_LOD_BIAS, -0.6f);
}

TextureArray2D::TextureArray2D()
{
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_handle);
}

TextureArray2D& TextureArray2D ::operator=(TextureArray2D&& other) noexcept
{
    m_handle = other.m_handle;
    m_textureDims = other.m_textureDims;
    m_textureCount = other.m_textureCount;
    m_maxTextures = other.m_maxTextures;
    other.m_handle = 0;
    other.m_textureDims = 0;
    other.m_textureCount = 0;
    other.m_maxTextures = 0;
    return *this;
}

TextureArray2D ::TextureArray2D(TextureArray2D&& other) noexcept
    : m_handle{other.m_handle}
    , m_textureDims{other.m_textureDims}
    , m_textureCount{other.m_textureCount}
    , m_maxTextures{other.m_maxTextures}
{
    other.m_handle = 0;
    other.m_textureDims = 0;
    other.m_textureCount = 0;
    other.m_maxTextures = 0;
}

TextureArray2D::~TextureArray2D()
{
    glDeleteTextures(1, &m_handle);
}

void TextureArray2D::create(GLuint textureDims, GLuint textureCount)
{
    m_textureDims = textureDims;
    m_maxTextures = textureCount;

    glTextureStorage3D(m_handle, 1, GL_RGBA8, textureDims, textureDims, textureCount);

    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

GLuint TextureArray2D::addTexture(const char* file)
{
    if (m_textureCount == m_maxTextures) {
        std::cout << "Cannot add anymore textures, maxmium has been reached.\n";
        return 0;
    }
    char texturePath[128] = "Data/Textures/";
    std::strcat(texturePath, file);

    sf::Image img;
    if (!img.loadFromFile(texturePath)) {
        return 0;
    }

    glTextureSubImage3D(m_handle, 0, 0, 0, m_textureCount, m_textureDims, m_textureDims, 1, GL_RGBA, GL_UNSIGNED_BYTE,
                        img.getPixelsPtr());

    return m_textureCount++;
}

void TextureArray2D::bind() const
{
    glBindTextureUnit(0, m_handle);
}