#include "application.h"

#include <cmath>
#include <iostream>

#include "gl/primitive.h"

Application::Application(sf::Window &window)
    : m_window(window)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    m_window.setFramerateLimit(60);
    m_window.setKeyRepeatEnabled(false);

    glViewport(0, 0, 1280, 720);
    glEnable(GL_DEPTH_TEST);

    m_quadShader.program.create("static", "static");
    m_quadShader.program.bind();
    m_quadShader.projViewLocation =
        m_quadShader.program.getUniformLocation("projectionViewMatrix");
    m_quadShader.modelLocation =
        m_quadShader.program.getUniformLocation("modelMatrix");

    m_quad = makeQuadVertexArray(1.0f, 1.0f);

    m_projectionMatrix =
        glm::perspective(3.14f / 2.0f, 1280.0f / 720.0f, 0.01f, 100.0f);
}

void Application::run()
{
    while (m_window.isOpen()) {
        sf::Event e;
        while (m_window.pollEvent(e)) {
            onEvent(e);
        }

        onInput();

        onUpdate();

        // Render
        glClearColor(0.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        onRender();

        m_window.display();
    }
}

void Application::onEvent(sf::Event e)
{
    m_keyboard.update(e);
    switch (e.type) {
        case sf::Event::Closed:
            m_window.close();
            break;

        case sf::Event::KeyReleased:
            switch (e.key.code) {
                case sf::Keyboard::Escape:
                    m_window.close();
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}

void Application::onInput()
{
    static auto lastMousePosition = sf::Mouse::getPosition(m_window);
    auto change = sf::Mouse::getPosition(m_window) - lastMousePosition;
    player.rot.x += static_cast<float>(change.y / 10);
    player.rot.y += static_cast<float>(change.x / 10);
    sf::Mouse::setPosition({static_cast<int>(m_window.getSize().x / 2),
                            static_cast<int>(m_window.getSize().y / 2)},
                           m_window);
    lastMousePosition = sf::Mouse::getPosition(m_window);
    player.rot.x = glm::clamp(player.rot.x, -170.0f, 170.0f);

    // Input
    const float SPEED = 0.05f;
    if (m_keyboard.isKeyDown(sf::Keyboard::W)) {
        player.pos.x += -glm::cos(glm::radians(player.rot.y + 90)) * SPEED;
        player.pos.z += -glm::sin(glm::radians(player.rot.y + 90)) * SPEED;
    }
    else if (m_keyboard.isKeyDown(sf::Keyboard::S)) {
        player.pos.x += glm::cos(glm::radians(player.rot.y + 90)) * SPEED;
        player.pos.z += glm::sin(glm::radians(player.rot.y + 90)) * SPEED;
    }
    if (m_keyboard.isKeyDown(sf::Keyboard::A)) {
        player.pos.x += -glm::cos(glm::radians(player.rot.y)) * SPEED;
        player.pos.z += -glm::sin(glm::radians(player.rot.y)) * SPEED;
    }
    else if (m_keyboard.isKeyDown(sf::Keyboard::D)) {
        player.pos.x += glm::cos(glm::radians(player.rot.y)) * SPEED;
        player.pos.z += glm::sin(glm::radians(player.rot.y)) * SPEED;
    }
}

void Application::onUpdate()
{
}

void Application::onRender()
{
    m_quadShader.program.bind();
    glm::mat4 viewMatrix{1.0f};
    viewMatrix = glm::rotate(viewMatrix, glm::radians(player.rot.x), {1, 0, 0});
    viewMatrix = glm::rotate(viewMatrix, glm::radians(player.rot.y), {0, 1, 0});
    viewMatrix = glm::rotate(viewMatrix, glm::radians(player.rot.z), {0, 0, 1});
    viewMatrix = glm::translate(viewMatrix, -player.pos);

    glm::mat4 projectionViewMatrix{1.0f};
    projectionViewMatrix = m_projectionMatrix * viewMatrix;

    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::rotate(modelMatrix, 3.14f / 4.0f, {1.0, 0.0, 0.0});

    gl::loadUniform(m_quadShader.projViewLocation, projectionViewMatrix);
    gl::loadUniform(m_quadShader.modelLocation, modelMatrix);

    // Render
    m_quad.bind();
    m_quad.getDrawable().bindAndDraw();
}