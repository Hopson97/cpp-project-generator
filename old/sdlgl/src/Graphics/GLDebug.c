#include "GLDebug.h"

#include <glad/glad.h>
#include <stdio.h>

static void GLAPIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id,
                                       GLenum severity, GLsizei length,
                                       const GLchar* message, const void* userParam)
{
    // clang-format off
    const char* severityString = "?";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:            severityString = "high";    break;
        case GL_DEBUG_SEVERITY_MEDIUM:          severityString = "medium";  break;
        case GL_DEBUG_SEVERITY_LOW:             severityString = "low";     break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:                                return;
    }

    const char* sourceString = "?";
    switch (source) {
        case GL_DEBUG_SOURCE_API:               sourceString = "API";               break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     sourceString = "window system";     break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:   sourceString = "shader compiler";   break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:       sourceString = "third party";       break;
        case GL_DEBUG_SOURCE_APPLICATION:       sourceString = "app";               break;
        case GL_DEBUG_SOURCE_OTHER:             sourceString = "other";             break;
    }

    const char* typeString = "?";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:                   typeString = "error";               break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:     typeString = "deprecated behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:      typeString = "undefined behavior";  break;
        case GL_DEBUG_TYPE_PORTABILITY:             typeString = "portability";         break;
        case GL_DEBUG_TYPE_MARKER:                  typeString = "marker";              break;
        case GL_DEBUG_TYPE_PUSH_GROUP:              typeString = "push group";          break;
        case GL_DEBUG_TYPE_POP_GROUP:               typeString = "pop group";           break;
        case GL_DEBUG_TYPE_OTHER:                   typeString = "other";               break;
    }
    // clang-format on

    fprintf(stderr,
            "OpenGL Message.\n Type: %s\nSeverity: %s\nSource: %s\nMessage: %s\n\n",
            typeString, severityString, sourceString, message);
}

void initGLDebug()
{
#ifndef __APPLE__
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // disable if in release
#endif
    glDebugMessageCallback(glDebugCallback, NULL);

    // this disables messages printed for successfully compiled shaders
    glDebugMessageControl(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DEBUG_TYPE_OTHER,
                          GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
}
