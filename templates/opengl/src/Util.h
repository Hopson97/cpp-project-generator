#pragma once

#include <SFML/Window/Window.hpp>
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif // _WIN32

// General random helper functions

bool initOpenGlWindow(sf::Window& window, unsigned width, unsigned height);

void glCheckError(const char* file, unsigned int line, const char* expression);

// This is OpenGL checking code from
// https://github.com/SFML/SFML/blob/master/src/SFML/Graphics/GLCheck.hpp
#ifdef NDEBUG
#define glCheck(expr)                                                                    \
    expr;                                                                                \
    if (!GLAD_GL_KHR_debug) {                                                            \
        glCheckError(__FILE__, __LINE__, #expr)                                          \
    };
#else
#define glCheck(call) call
#endif

std::string loadFileToString(const std::string& path);