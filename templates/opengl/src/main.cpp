#include "GL/Shader.h"
#include "GL/Texture.h"
#include "GL/VertexArray.h"
#include "Keyboard.h"
#include "Maths.h"
#include <SFML/Window/Window.hpp>
#include <glad/glad.h>
#include <iostream>
#include "Util.h"
#include <array>

int main()
{
    // Init Window, OpenGL set up etc
    sf::Window window;
    if (!initOpenGlWindow(window, 1280, 720)) {
        return 1;
    }

    // Create a shader
    glpp::Shader shader;
    shader.addShader("min_vertex", glpp::ShaderType::Vertex);
    shader.addShader("min_fragment", glpp::ShaderType::Fragment);
    shader.linkShaders();
    shader.bind();
    auto locModelMatrix = shader.getUniformLocation("modelMatrix");
    auto locPVMatrix = shader.getUniformLocation("projectionViewMatrix");
    auto locLightPos = shader.getUniformLocation("lightPosition");

    // Create a vertex array
    std::vector<GLfloat> positions = {-0.5, -0.5, 0.0, 0.5,  -0.5, 0.0f,
                                      0.5,  0.5,  0.0, -0.5, 0.5,  0.0};
    std::vector<GLfloat> texCoords = {0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0};
    std::vector<GLfloat> normals = {0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.0f};
    std::vector<GLuint> indices = {0, 1, 2, 2, 3, 0};
    glpp::VertexArray quad;
    quad.bind();
    quad.addAttribute(positions, 3);
    quad.addAttribute(texCoords, 2);
    quad.addAttribute(normals, 3);
    quad.addElements(indices);

    // Load a texture
    glpp::Texture2d texture;
    texture.create("opengl_logo.png", true);
    texture.bind();

    // Some variables I guess
    glm::vec3 playerPosition{0.0f};
    glm::vec3 playerRotation{0.0f};
    glm::vec3 modelLocation{0.0, 0.0f, -1.0f};
    glm::vec3 modelRotation{0.0, 45.0f, 30.0f};
    auto projection = createProjectionMatrix(1280.0f / 720.0f, 75.0f);

    const float PLAYER_SPEED = 0.01f;
    Keyboard keyboard;
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            keyboard.update(e);
            switch (e.type) {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    switch (e.key.code) {
                        case sf::Keyboard::Escape:
                            window.close();
                            break;

                        default:
                            break;
                    }

                default:
                    break;
            }
        }

        // Input stuff
        static auto lastMousePosition = sf::Mouse::getPosition(window);
        auto change = sf::Mouse::getPosition(window) - lastMousePosition;
        playerRotation.x += static_cast<float>(change.y / 8.0f * 0.5);
        playerRotation.y += static_cast<float>(change.x / 8.0f * 0.5);
        sf::Mouse::setPosition({(int)window.getSize().x / 2, (int)window.getSize().y / 2},
                               window);
        lastMousePosition.x = (int)window.getSize().x / 2;
        lastMousePosition.y = (int)window.getSize().y / 2;

        if (keyboard.isKeyDown(sf::Keyboard::W)) {
            playerPosition += forwardsVector(playerRotation) * PLAYER_SPEED;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::S)) {
            playerPosition += backwardsVector(playerRotation) * PLAYER_SPEED;
        }
        if (keyboard.isKeyDown(sf::Keyboard::A)) {
            playerPosition += leftVector(playerRotation) * PLAYER_SPEED;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::D)) {
            playerPosition += rightVector(playerRotation) * PLAYER_SPEED;
        }

        // Draw stuff
        auto modelmatrix = createModelMatrix(modelLocation, modelRotation);
        auto viewmatrix = createViewMartix(playerPosition, playerRotation);
        glpp::loadUniform(locModelMatrix, modelmatrix);
        glpp::loadUniform(locPVMatrix, projection * viewmatrix);
        glpp::loadUniform(locLightPos, {1.0, 1.0, 1.0});

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        quad.getDrawable().draw();

        window.display();
    }
}