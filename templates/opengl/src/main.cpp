#include "GUI.h"
#include "Game.h"
#include "Graphics/GLWrappers.h"
#include "Utility.h"
#include <SFML/GpuPreference.hpp>

int main()
{
    bool isMouseActive = true;

    sf::Window window;
    if (!initWindow(&window)) {
        return 1;
    }
    window.setFramerateLimit(60);
    guiInit(window);

    Game game;

    Keyboard keyboard;
    while (window.isOpen()) {
        guiBeginFrame();
        sf::Event e;
        while (window.pollEvent(e)) {
            keyboard.update(e);
            guiProcessEvent(e);
            if (e.type == sf::Event::KeyReleased) {
                if (e.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (e.key.code == sf::Keyboard::L) {
                    isMouseActive = !isMouseActive;
                    window.setMouseCursorVisible(!isMouseActive);
                }
            }
        }

        game.onInput(keyboard, window, isMouseActive);
        game.onUpdate();

        glEnable(GL_DEPTH_TEST);
        game.onRender();
        game.onGUI();
        guiEndFrame();

        window.display();
    }
    guiShutdown();
}