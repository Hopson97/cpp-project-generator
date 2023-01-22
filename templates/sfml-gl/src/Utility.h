#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <array>

// Default window height and width
#define WIDTH 1600
#define HEIGHT 900

char* getFileContent(const char* fileName);

// Colour Util
struct Colour {
    uint8_t red = 255;
    uint8_t green = 255;
    uint8_t blue = 255;
    uint8_t alpha = 255;
};

enum ColourSetMode {
    COL_SET_BG = 48,
    COL_SET_FG = 38,
};

class Keyboard {
  public:
    Keyboard()
    {
        std::fill(m_keys.begin(), m_keys.end(), false);
    }

    bool isKeyDown(sf::Keyboard::Key key) const
    {
        return m_keys[key];
    }

    void update(sf::Event e)
    {
        if (e.type == sf::Event::KeyPressed) {
            m_keys[e.key.code] = true;
        }
        else if (e.type == sf::Event::KeyReleased) {
            m_keys[e.key.code] = false;
        }
    }

  private:
    std::array<bool, sf::Keyboard::KeyCount> m_keys{false};
};

void setTerminalBackgroundColour(Colour colour);
void setTerminalTextColour(Colour colour);

void setBackgroundColourRGB(uint8_t red, uint8_t green, uint8_t blue);
void setTextColourRGB(uint8_t red, uint8_t green, uint8_t blue);

bool initWindow(sf::Window* window);

const Colour COLOUR_SKY_BLUE = {135, 206, 235, 255};
const Colour COLOUR_SAND = {235, 214, 135, 255};
void setClearColour(Colour colour);
