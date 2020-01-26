#pragma once

#include <SFML/Window/Window.hpp>
#include <iostream>
#include <vector>

#include "gl/primitive.h"
#include "gl/shader.h"
#include "gl/vertex_array.h"

#include "input/keyboard.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr float PI = 3.14159;
constexpr float ACCELERATION = 0.4f;
constexpr float ACCELERATION_DAMP = 0.85f;

class Application {
  public:
    Application(sf::Window &window);

    void run();

  private:
    void onEvent(sf::Event e);
    void onInput();
    void onUpdate();
    void onRender();

    sf::Window &m_window;

    struct {
        gl::Shader program;
        gl::UniformLocation projViewLocation;
        gl::UniformLocation modelLocation;
    } m_quadShader;

    struct {
        glm::vec3 pos{0.0, 0.0, 2.0f}, rot;
    } player;

    glm::mat4 m_projectionMatrix{1.0f};

    gl::VertexArray m_quad;

    Keyboard m_keyboard;
};