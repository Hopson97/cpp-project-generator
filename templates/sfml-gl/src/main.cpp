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

    Game game(window.getSize().x, window.getSize().y);

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
            else if (e.type == sf::Event::Closed) {
                window.close();
            }
            else if (e.type == sf::Event::Resized) {
                glViewport(0, 0, e.size.width, e.size.height);
                game.onResize(e.size.width, e.size.height);
            }
        }

        game.onInput(keyboard, window, isMouseActive);
        game.onUpdate();

        glEnable(GL_DEPTH_TEST);
        game.onRender(window);
        game.onGUI();
        guiEndFrame();

        window.display();
    }
    guiShutdown();
}