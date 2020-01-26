#include <SFML/Window/Window.hpp>
#include <iostream>
#include <vector>

#include "application.h"
#include "gl/primitive.h"
#include "gl/shader.h"
#include "gl/vertex_array.h"

#include "input/keyboard.h"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <stdexcept>

/*
    std::vector<GLfloat> points{
        -0.5f, -0.5f, 0.0f, 0.5f,  -0.5f, 0.0f,
        0.5f,  0.5f,  0.0f, -0.5f, 0.5f,  0.0f,
    };

    std::vector<GLuint> indices{0, 1, 2, 2, 3, 0};

    std::vector<GLfloat> textureCoords{0.0f, 0.0f, 1.0f, 0.0f,
                                       1.0f, 1.0f, 0.0f, 1.0f};
*/

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    sf::Window window({1280, 720}, "OpenGL", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    if (!gladLoadGL()) {
        std::cout << "Unable to load OpenGL functions, exiting\n";
        return -1;
    }
    Application app(window);

    app.run();
}