#include "Keyboard.h"

Keyboard::Keyboard()
{
    std::fill(m_keys.begin(), m_keys.end(), false);
}

void Keyboard::update(sf::Event e)
{
    switch (e.type) {
        case sf::Event::KeyPressed:
            m_keys[e.key.code] = true;
            break;

        case sf::Event::KeyReleased:
            m_keys[e.key.code] = false;

        default:
            break;
    }
}

bool Keyboard::isKeyDown(sf::Keyboard::Key key) const
{
    return m_keys[key];
}

void Keyboard::resetKeys()
{
    std::fill(m_keys.begin(), m_keys.end(), false);
}