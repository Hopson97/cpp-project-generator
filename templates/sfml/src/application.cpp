#include "application.h"

#include <cmath>
#include <iostream>

Application::Application()
    : m_window({WINDOW_WIDTH, WINDOW_HEIGHT}, "Collision Testing")
{
    m_window.setFramerateLimit(60);
    m_window.setKeyRepeatEnabled(false);

    m_playerTexture.loadFromFile("res/person.png");

    m_sprite.setSize({64, 64});
    m_sprite.setOrigin({64 / 2, 64 / 2});
    m_sprite.setTexture(&m_playerTexture);
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

        m_window.clear();
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

        default:
            break;
    }
}

void Application::onInput()
{
    auto mousePosition = sf::Mouse::getPosition(m_window);
    auto playerPosition = m_sprite.getPosition();
    auto dx = mousePosition.x - playerPosition.x;
    auto dy = mousePosition.y - playerPosition.y;
    auto rotation = std::atan2(dy, dx);

    m_sprite.setRotation(rotation * 180 / PI + 90);

    if (m_keyboard.isKeyDown(sf::Keyboard::W)) {
        auto rads = (m_sprite.getRotation() + 90) * PI / 180;
        m_velocity.x -= std::cos(rads) * ACCELERATION;
        m_velocity.y -= std::sin(rads) * ACCELERATION;
    }
    else if (m_keyboard.isKeyDown(sf::Keyboard::S)) {
        auto rads = (m_sprite.getRotation() + 90) * PI / 180;
        m_velocity.x += std::cos(rads) * ACCELERATION;
        m_velocity.y += std::sin(rads) * ACCELERATION;
    }

    if (m_keyboard.isKeyDown(sf::Keyboard::A)) {
        auto rads = (m_sprite.getRotation()) * PI / 180;
        m_velocity.x -= std::cos(rads) * ACCELERATION;
        m_velocity.y -= std::sin(rads) * ACCELERATION;
    }
    else if (m_keyboard.isKeyDown(sf::Keyboard::D)) {
        auto rads = (m_sprite.getRotation()) * PI / 180;
        m_velocity.x += std::cos(rads) * ACCELERATION;
        m_velocity.y += std::sin(rads) * ACCELERATION;
    }
}

void Application::onUpdate()
{
    m_sprite.move(m_velocity);
    m_velocity *= ACCELERATION_DAMP;
}

void Application::onRender()
{

    m_window.draw(m_sprite);
}