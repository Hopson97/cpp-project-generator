#pragma once

#include "GUI.h"
#include "Graphics/GLWrappers.h"
#include "Maths.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Window.hpp>
#include <atomic>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class Keyboard;

class Game {

  public:
    Game();

    void onInput(const Keyboard& keyboard, const sf::Window& window, bool isMouseActive);

    void onUpdate();

    void onRender();

    void onGUI();

  private:
    void renderScene(const glm::mat4& projectionViewMatrix);

    Shader m_sceneShader;
    VertexArray m_terrain;
    VertexArray m_lightCube;
    Texture2D m_texture;

    ViewFrustum m_frustum;
    Transform m_cameraTransform;
    Transform m_lightTransform;

    glm::mat4 m_projectionMatrix;

    sf::Clock m_timer;
};
