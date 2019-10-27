#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "input/keyboard.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr float PI = 3.14159;
constexpr float ACCELERATION = 0.4f;
constexpr float ACCELERATION_DAMP = 0.85f;

class Application {
  public:
    Application();

    void run();

  private:
    void onEvent(sf::Event e);
    void onInput();
    void onUpdate();
    void onRender();

    sf::RectangleShape m_sprite;
    sf::Vector2f m_velocity;
    sf::RenderWindow m_window;
    sf::Texture m_playerTexture;

    Keyboard m_keyboard;
};