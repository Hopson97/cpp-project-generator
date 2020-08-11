#include "Util.h"
#include <cstdio>
#include <cstdlib>
#include <glad/glad.h>

namespace {
    void GLAPIENTRY glDebugCallback(GLenum source, GLenum type, GLuint _, GLenum severity,
                                    GLsizei __, const char* message, const void* ___)
    {
        (void)_;
        (void)__;
        (void)___;
        // clang-format off
    const char* severity_str = "?";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:    severity_str = "high";      break;
        case GL_DEBUG_SEVERITY_MEDIUM:  severity_str = "medium";    break;
        case GL_DEBUG_SEVERITY_LOW:     severity_str = "low";       break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:                        return;
    }

    const char* src = "?";
    switch (source) {
        case GL_DEBUG_SOURCE_API:               src = "API";                break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     src = "window system";      break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   src = "shader compiler";    break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       src = "third party";        break;
        case GL_DEBUG_SOURCE_APPLICATION:       src = "app";                break;
        case GL_DEBUG_SOURCE_OTHER:             src = "other";              break;
    }

    const char* type_str = "?";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               type_str = "error";                 break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_str = "deprecated behavior";   break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_str = "undefined behavior";    break;
        case GL_DEBUG_TYPE_PORTABILITY:         type_str = "portability";           break;
        case GL_DEBUG_TYPE_MARKER:              type_str = "marker";                break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          type_str = "push group";            break;
        case GL_DEBUG_TYPE_POP_GROUP:           type_str = "pop group";             break;
        case GL_DEBUG_TYPE_OTHER:               type_str = "other";                 break;
    }
        // clang-format on

        std::printf(
            "OpenGL Message\n Type: %s\n Severity: %s\n Source: %s\n Message: %s\n\n",
            type_str, severity_str, src, message);
    }

    void initGLDebug()
    {
#ifndef __APPLE__
#ifndef NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
        glDebugMessageCallback(glDebugCallback, NULL);

        // this disables messages printed for successfully compiled shaders
        glDebugMessageControl(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DEBUG_TYPE_OTHER,
                              GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
    }
} // namespace

bool initOpenGlWindow(sf::Window& window, unsigned width, unsigned height)
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.stencilBits = 8;
    contextSettings.antialiasingLevel = 4;
    contextSettings.majorVersion = 3;
    contextSettings.minorVersion = 3;
    contextSettings.attributeFlags = sf::ContextSettings::Core;
    window.create({width, height}, "OpenGL", sf::Style::Close, contextSettings);
    window.setFramerateLimit(60);
    if (!gladLoadGL()) {
        std::printf("Failed to load OpenGL, exiting.\n");
        return false;
    }
    initGLDebug();

    glCheck(glClearColor(0.1f, 0.0f, 0.0f, 0.0f));
    glCheck(glViewport(0, 0, window.getSize().x, window.getSize().y));
    glCheck(glEnable(GL_DEPTH_TEST));
    glCheck(glEnable(GL_CULL_FACE));
    glCheck(glCullFace(GL_BACK));

    return true;
}

void glCheckError(const char* file, unsigned int line, const char* expression)
{
    GLenum errorCode = glGetError();

    while (errorCode != GL_NO_ERROR) {
        const char* error = "Unknown error";
        const char* description = "No description";

        switch (errorCode) {
            case GL_INVALID_ENUM: {
                error = "GL_INVALID_ENUM";
                description = "An unacceptable value has been specified "
                              "for an enumerated argument.";
                break;
            }

            case GL_INVALID_VALUE: {
                error = "GL_INVALID_VALUE";
                description = "A numeric argument is out of range.";
                break;
            }

            case GL_INVALID_OPERATION: {
                error = "GL_INVALID_OPERATION";
                description = "The specified operation is not allowed in "
                              "the current state.";
                break;
            }

            case GL_OUT_OF_MEMORY: {
                error = "GL_OUT_OF_MEMORY";
                description = "There is not enough memory left to execute "
                              "the command.";
                break;
            }
        }

        std::printf(
            "OpenGL Error\n Source: %s\n Line: %d\n Expression: %s\n Message: %s\n %s "
            "\n\n",
            file, line, expression, error, description);
        exit(-1);
    }
}

std::string loadFileToString(const std::string& path)
{
    std::FILE* fp = std::fopen(path.c_str(), "r");
    if (!fp) {
        return "";
    }

    std::fseek(fp, 0, SEEK_END);
    auto length = std::ftell(fp);
    std::rewind(fp);

    std::string buff;
    buff.resize(length);
    std::fread(&buff[0], 1, length, fp);

    std::fclose(fp);
    return buff;
}