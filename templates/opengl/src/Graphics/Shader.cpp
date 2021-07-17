#include "Shader.h"

#include "../Utility.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

static const char* getShaderString(GLenum shaderType)
{
    if (shaderType == GL_VERTEX_SHADER) {
        return "Vertex Shader";
    }
    else if (shaderType == GL_FRAGMENT_SHADER) {
        return "Fragment Shader";
    }
    else {
        return "Unknown shader";
    }
}

static bool compileShader(GLuint* shaderOut, const char* source, GLenum shaderType)
{
    if (!source) {
        fprintf(stderr, "Failed to load %s file.\n", getShaderString(shaderType));
        return false;
    }
    GLuint shader = glCreateShader(shaderType);

    int length = strlen(source);
    glShaderSource(shader, 1, (const GLchar* const*)&source, &length);
    glCompileShader(shader);

    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        char buff[1024];
        glGetShaderInfoLog(shader, 1024, NULL, buff);
        fprintf(stderr, "%s compilation failed: %s\n", getShaderString(shaderType), buff);
        return false;
    }
    *shaderOut = shader;
    return true;
}

Shader& Shader::operator=(Shader&& other)
{
    m_program = other.m_program;
    other.m_program = 0;
    return *this;
}

Shader::Shader(Shader&& other)
    : m_program{other.m_program}
{
    other.m_program = 0;
}

Shader::~Shader()
{
    if (m_program != 0) {
        glDeleteProgram(m_program);
    }
}

void Shader::loadFromMemory(const char* vertexSource, const char* fragmentSource)
{
    GLuint vertexShader;
    GLuint fragmentShader;
    if (!compileShader(&vertexShader, vertexSource, GL_VERTEX_SHADER)) {
        exit(1);
    }

    if (!compileShader(&fragmentShader, fragmentSource, GL_FRAGMENT_SHADER)) {
        exit(1);
    }

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);

    glBindAttribLocation(m_program, 0, "inPosition");

    GLint status;
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        char buff[1024];
        glGetShaderInfoLog(m_program, 1024, NULL, buff);
        fprintf(stderr, "Failed to link shader programs: %s\n", buff);
        exit(1);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::loadFromFile(const char* vertexFilename, const char* fragmentFileName)
{

    char vertfullFileName[128] = "Data/Shaders/";
    char fragfullFileName[128] = "Data/Shaders/";
    strcat(vertfullFileName, vertexFilename);
    strcat(fragfullFileName, fragmentFileName);

    char* vertexSource = getFileContent(vertfullFileName);
    char* fragmentSource = getFileContent(fragfullFileName);

    loadFromMemory(vertexSource, fragmentSource);

    free(vertexSource);
    free(fragmentSource);
}

void Shader::bind() const
{
    assert(m_program != 0);
    glUseProgram(m_program);
}

GLuint Shader::getUniformLocation(const char* name)
{
    auto itr = m_uniformLocations.find(name);
    if (itr == m_uniformLocations.cend()) {
        GLint location = glGetUniformLocation(m_program, name);
        // std::cout << "Getting uniform location: " << name << std::endl;
        assert(location != -1);
        m_uniformLocations.emplace(name, location);
    }
    return m_uniformLocations[name];
}

void Shader::set(const char* name, int value)
{
    glUniform1i(getUniformLocation(name), value);
}

void Shader::set(const char* name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void Shader::set(const char* name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set(const char* name, const glm::vec4& vector)
{
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(vector));
}

void Shader::set(const char* name, const glm::vec3& vector)
{
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(vector));
}

void loadMatrix4ToShader(GLuint shader, const char* name, const glm::mat4& matrix)
{
    GLuint location = glGetUniformLocation(shader, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
